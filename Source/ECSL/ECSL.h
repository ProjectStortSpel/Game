#ifndef ECSL_H
#define ECSL_H

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#else
#endif

#include "Framework/Common/BitSet.h"
#include "Framework/Components/ComponentType.h"
#include "Framework/Components/DataManager.h"
#include "Framework/Components/Tables/DataArray.h"
#include "Framework/Components/Tables/DataMap.h"
#include "Framework/Components/Tables/DataTable.h"
#include "Framework/Components/Tables/EntityTable.h"
#include "Framework/Multithreading/Scheduler.h"
#include "Framework/Parsing/ComponentTypeReader.h"
#include "Framework/Parsing/Parser.h"
#include "Framework/Systems/SystemWorkGroup.h"
#include "Framework/Systems/SystemManager.h"
#include "Framework/Systems/Messaging/Message.h"
#include "Framework/World.h"
#include "Framework/WorldCreator.h"

#include "Interfaces/System.h"

#include "Managers/ComponentTypeManager.h"

#endif