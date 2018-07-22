#pragma once
//
//

#include "Program.h"
#include "globalincs/pstypes.h"

namespace actions {

/**
 * @brief Program manager class
 *
 * This singleton class manages all currently running program instances.
 */
class ProgramManager {
	SCP_vector<ProgramInstance> _runningPrograms;

	ProgramManager();
  public:
	/**
	 * @brief Execute all programs until an action wants to wait
	 */
	void stepInstances();

	/**
	 * @brief Adds a program instance to this manager so that it is executed on the next step call.
	 * @param instance
	 */
	void addInstance(ProgramInstance instance);

	static ProgramManager* instance();
};

} // namespace actions
