#ifndef ECSLWORKITEM_H
#define ECSLWORKITEM_H

#include "MPL/Framework/Tasks/WorkItem.h"

namespace ECSL
{
	struct ECSLWorkItem : public MPL::WorkItem
	{
		ECSLWorkItem() { }
		~ECSLWorkItem() { }

		unsigned int SystemId;
	};
}

#endif