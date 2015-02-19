#include "ParticleSystem.h"

using namespace Renderer;

ParticleSystem::ParticleSystem(std::string type, const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg)
{
	m_pos = _pos;
	m_nrParticles = _nParticles;
	m_scale = _scale;
	m_spriteSize = _spriteSize;
	m_lifeTime = _lifeTime;
	m_textureHandle = _texHandle;
	m_shader = _shaderProg;
	m_drawBuf = 1;
	m_color = _color;
	m_endPhase = false;

	if (type == "fire")
		CreateFire();
	else if (type == "smoke")
		CreateSmoke();

	//set uniforms?
	subRoutineUpdate = glGetSubroutineIndex(m_shader->GetShaderProgram(), GL_VERTEX_SHADER, "update");
	subRoutineRender = glGetSubroutineIndex(m_shader->GetShaderProgram(), GL_VERTEX_SHADER, "render");
	
	m_elapsedTime = 0.0f;
	m_removeDelayTime = 0.0f;
}

ParticleSystem::~ParticleSystem()
{
	glDeleteTransformFeedbacks(2, m_feedback);
	glDeleteBuffers(8, vboHandles);
	glDeleteVertexArrays(2, m_particleArray);
}

void ParticleSystem::CreateFire()
{
	// Create and allocate buffers A and B for m_posBuf, m_velBuf and m_startTime
	
	m_dstBlendFactor = GL_CONSTANT_COLOR;
	float scale = m_scale;

	m_accel = vec3(0.0);
	vec3 v(0.0f);
	float velocity, theta, phi;
	float mtime = 0.0f, rate = (m_lifeTime / (float)m_nrParticles);//0.00075f;

	GLfloat *posData = new GLfloat[m_nrParticles * 3];
	GLfloat *velData = new GLfloat[m_nrParticles * 3];
	GLfloat *timeData = new GLfloat[m_nrParticles];
	GLfloat *initVelData;
	GLfloat *initPosData;

	srand(time(NULL));
	for (GLuint i = 0; i < m_nrParticles; i++) {
		vec3 pos;
		pos = vec3(((float)(rand() % 9) - 4), 0.0f, ((float)(rand() % 9 - 4)));
		pos = glm::normalize(pos) * ((float)(rand() % 9 - 4)) * 0.5f * scale;

		posData[3 * i] = pos.x; //((float)(rand() % 9) - 4) * scale;
		posData[3 * i + 1] = pos.y; // 0.0;
		posData[3 * i + 2] = pos.z; // ((float)(rand() % 9 - 4)) * scale;

		// Pick the direction of the velocity
		theta = glm::mix(0.0f, (float)M_PI / 6.0f, (float)(rand() % 101) / 100);
		phi = glm::mix(0.0f, (float)(2 * M_PI), (float)(rand() % 101) / 100);
		
		v.x = sinf(theta) * cosf(phi) * 0.1; 
		if ((posData[3 * i] < 0 && v.x < 0) || (posData[3 * i] > 0 && v.x > 0))
			v.x *= -1;
		v.y = cosf(theta) * 0.2;
		v.z = sinf(theta) * sinf(phi) * 0.1;
		if ((posData[3 * i + 2] < 0 && v.z < 0) || (posData[3 * i + 2] > 0 && v.z > 0))
			v.z *= -1;

		// Scale to set the magnitude of the velocity (speed)
		velocity = glm::mix(1.25f, 1.5f, (float)(rand() % 101) / 100) * 0.0012;
		v = v * velocity;
		velData[3 * i] = v.x;
		velData[3 * i + 1] = v.y;
		velData[3 * i + 2] = v.z;

		timeData[i] = mtime;
		mtime += rate;

		//printf("velData[i]: %f, %f, %f \n", velData[3 * i], velData[3 * i + 1], velData[3 * i + 2]);
		//printf("timeData[i]: %f \n", timeData[i]);

	}
	initVelData = velData;
	initPosData = posData;

	
	glGenBuffers(8, vboHandles);
	m_posBuf[0] = vboHandles[0];
	m_posBuf[1] = vboHandles[1];
	m_velBuf[0] = vboHandles[2];
	m_velBuf[1] = vboHandles[3];
	m_startTime[0] = vboHandles[4];
	m_startTime[1] = vboHandles[5];
	m_initVelBuf = vboHandles[6];
	m_initPosBuf = vboHandles[7];

	//Create the two vertex arrays
	glGenVertexArrays(2, m_particleArray);
	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(m_particleArray[i]);

		// enable "vertex attribute arrays"
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	glBindVertexArray(0);

	//kanske ska vara glBufferData här ------   OBS!  --------
	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), posData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), velData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glBufferData(GL_ARRAY_BUFFER, m_nrParticles * sizeof(float), timeData, GL_STATIC_DRAW);

	}
	glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), initVelData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), initPosData, GL_STATIC_DRAW);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, m_feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[1]);

	delete posData;
	delete velData;
	delete timeData; 
	initVelData = 0;
	initPosData = 0;
}

void ParticleSystem::CreateSmoke()
{
	// Create and allocate buffers A and B for m_posBuf, m_velBuf and m_startTime
	m_dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
	float scale = m_scale;
	m_accel = vec3(0.0f, 0.0f, 0.0f) * scale;
	vec3 v(0.0f);
	float velocity, theta, phi;
	float mtime = 0.0f, rate = (m_lifeTime / (float)m_nrParticles);//0.00075f;

	GLfloat *posData = new GLfloat[m_nrParticles * 3];
	GLfloat *velData = new GLfloat[m_nrParticles * 3];
	GLfloat *timeData = new GLfloat[m_nrParticles];
	GLfloat *initVelData;
	GLfloat *initPosData;

	srand(time(NULL));
	for (GLuint i = 0; i < m_nrParticles; i++) {
		vec3 pos;
		pos = vec3(((float)(rand() % 101) - 50.0), 0.0f, ((float)(rand() % 101) - 50.0));
		pos = glm::normalize(pos) * float(((float)(rand() % 21) / 10 - 1.0)) * 1.5f * scale;

		posData[3 * i] = pos.x;
		posData[3 * i + 1] = pos.y;
		posData[3 * i + 2] = pos.z;

		// Pick the direction of the velocity
		theta = glm::mix(0.0f, (float)M_PI / 6.0f, (float)(rand() % 101) / 100);
		phi = glm::mix(0.0f, (float)(2 * M_PI), (float)(rand() % 101) / 100);
		v.x = sinf(theta) * cosf(phi) * 0.1;
		v.y = cosf(theta) * 0.15;
		v.z = sinf(theta) * sinf(phi) * 0.1;
		// Scale to set the magnitude of the velocity (speed)
		velocity = glm::mix(1.25f, 1.5f, (float)(rand() % 101) / 100) * 0.0012f;
		v = v * velocity;
		velData[3 * i] = v.x;
		velData[3 * i + 1] = v.y;
		velData[3 * i + 2] = v.z;

		timeData[i] = mtime;
		mtime += rate;
	}
	initVelData = velData;
	initPosData = posData;

	GLuint vboHandles[8];
	glGenBuffers(8, vboHandles);
	m_posBuf[0] = vboHandles[0];
	m_posBuf[1] = vboHandles[1];
	m_velBuf[0] = vboHandles[2];
	m_velBuf[1] = vboHandles[3];
	m_startTime[0] = vboHandles[4];
	m_startTime[1] = vboHandles[5];
	m_initVelBuf = vboHandles[6];
	m_initPosBuf = vboHandles[7];

	//Create the two vertex arrays
	glGenVertexArrays(2, m_particleArray);
	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(m_particleArray[i]);

		// enable "vertex attribute arrays"
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	glBindVertexArray(0);

	//kanske ska vara glBufferData här ------   OBS!  --------
	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), posData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), velData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glBufferData(GL_ARRAY_BUFFER, m_nrParticles * sizeof(float), timeData, GL_STATIC_DRAW);

	}
	glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), initVelData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), initPosData, GL_STATIC_DRAW);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, m_feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[1]);

	delete posData;
	delete velData;
	delete timeData;
	initVelData = 0;
	initPosData = 0;
}

void ParticleSystem::Render(float _dt)
{
	glBlendColor(0.92, 0.92, 0.92, 1.0);
	glBlendFunc(GL_SRC_ALPHA, m_dstBlendFactor);

	float dt = 1000.f * (_dt);
	m_elapsedTime += dt;

	if (m_endPhase)
		m_removeDelayTime += dt;

	/////////// Update pass ////////////////
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &subRoutineUpdate);
	// Set the uniforms: H and Time
	m_shader->SetUniVariable("Time", glfloat, &m_elapsedTime);
	m_shader->SetUniVariable("DeltaTime", glfloat, &dt);
	m_shader->SetUniVariable("ParticleLifetime", glfloat, &m_lifeTime);
	m_shader->SetUniVariable("Size", glfloat, &m_spriteSize);
	m_shader->SetUniVariable("Accel", vector3, &m_accel);
	m_shader->SetUniVariable("EndPhase", glint, &m_endPhase);

	// Disable rendering
	glEnable(GL_RASTERIZER_DISCARD);

	// Bind the feedback object for the buffers to be drawn next
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[m_drawBuf]);

	// Draw points from input buffer with transform feedback
	glBeginTransformFeedback(GL_POINTS);
	glBindVertexArray(m_particleArray[1 - m_drawBuf]);
	glDrawArrays(GL_POINTS, 0, m_nrParticles);
	glEndTransformFeedback();

	// Enable rendering
	glDisable(GL_RASTERIZER_DISCARD);
	//////////// Render pass ///////////////
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &subRoutineRender);
	//glClear(GL_COLOR_BUFFER_BIT);
	// Initialize uniforms for transformation matrices if needed

	// Un-bind the feedback object.
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	// Draw the sprites from the feedback buffer
	glBindVertexArray(m_particleArray[m_drawBuf]);
	glDrawArrays(GL_POINTS, 0, m_nrParticles);
	// Swap buffers
	m_drawBuf = 1 - m_drawBuf;
}

void ParticleSystem::EnterEndPhase()
{
	m_endPhase = true;
}

bool ParticleSystem::ReadyToBeDeleted()
{
	if (m_removeDelayTime > m_lifeTime)
		return true;
	
	return false;
}