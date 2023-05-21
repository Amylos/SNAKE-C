/**
 * @file app.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief The generic app type definition
 *        Opaque type.
 */
typedef struct s_app t_app;

/**
 * @brief The application "constructor".
 *        Performs memory allocation, and
 *        components initialization.
 * 
 * @return t_app* pointer to the newly created application.
 */
t_app*AppNew(void);

/**
 * @brief The application "destructor".
 *        Performs memory de-allocation, and
 *        freeing all resources.
 * 
 * @param pApp pointer to the application to destroy.
 * @return t_app* NULL. 
 */
t_app*AppDel(t_app*pApp);

/**
 * @brief The main application loop.
 *        Processes all events from OS.
 * 
 * @param pApp pointer to the application to run.
 * @return int EXIT_SUCCES in case of a normal issue,
 *             EXIT_FAILURE otherwise.
 */
int AppRun(t_app*pApp);
