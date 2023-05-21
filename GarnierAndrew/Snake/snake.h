/**
 * @file snake.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef SNAKE_H_
#define SNAKE_H_

/**
 * @brief The snake direction enumeration.
 * 
 */
typedef enum e_direction{
	DIR_RIGHT = 0 ,
	DIR_LEFT,
	DIR_DOWN,
	DIR_UP,
	DIR_NONE
}t_direction;

/**
 * @brief The generic snake type definition.
 *        Opaque type.
 */
typedef struct s_snake t_snake;

/**
 * @brief The snake entity constructor.
 *        Performs memory allocation, and
 *        components initialization.
 *  
 * @param szNbSections	initial number of snake sections, head comprised.
 * @param iHeadPosX 	initial head X location.
 * @param iHeadPosY  	initial head Y location.
 * @param direction 	initial head direction.
 * @param pColor 		RGBA snake color.
 * 
 * @return t_snake*		pointer to the newly created snake. 	
 */
t_snake*SnakeNew(size_t szNbSections, int iHeadPosX, int iHeadPosY, t_direction direction, const SDL_Color*pColor);

/**
 * @brief The snake entity destructor.
 *        Performs memory de-allocation, and
 *        freeing all resources.
 * 
 * @param pSnake 	pointer to the snake to destroy.
 * 
 * @return t_snake* NULL pointer.
 */
t_snake*SnakeDel(t_snake*pSnake);

/**
 * @brief Performs snake drawing by drawing all snake sections.
 * 
 * @param pSnake 	pointer to the snake to draw.
 * @param pRenderer pointer to the graphical renderer to draw in.
 * @param pArea 	pointer to the drawing area.
 * 
 * @return t_snake*	pointer to the current snake entity. 
 */
t_snake*SnakeDraw(t_snake*pSnake, SDL_Renderer*pRenderer, const SDL_Rect*pArea);

/**
 * @brief Performs snake movements by perfoming movements of all snake sections.
 * 
 * @param pSnake 	pointer to the snake to move.
 * @param pArea 	pointer to the moving area.
 * 
 * @return t_snake*	pointer to the current snake entity. 
 */
t_snake*SnakeMove(t_snake*pSnake, const SDL_Rect*pArea);

/**
 * @brief Performs snake growing up by adding a section to the tail.
 * 
 * @param pSnake 	pointer to the snake to grow up.
 * 
 * @return t_snake*	pointer to the current snake entity. 
 */
t_snake*SnakeGrowup(t_snake*pSnake);


/**
 * @brief Changes the snake head direction.
 * 		  Change is made if it's not the opposite of the current
 * 		  head direction.
 * 
 * @param pSnake 	pointer to the snake to change the head direction.
 * @param direction the new direction to apply.
 * 
 * @return t_snake*	pointer to the current snake entity. 
 */
t_snake*SnakeChangeDirection(t_snake*pSnake, t_direction direction);

/**
 * @brief Tells if the snake head is located at passed coordinates prameters.
 * 
 * @param pSnake pointer to the snake to check.
 * @param iAtX 	 x coordinate to check.
 * @param iAtY 	 y coordinate to check.
 * 
 * @return int   a non nul value in case of match, zero otherwise.
 */
int SnakeIsHeadAt(const t_snake*pSnake, int iAtX, int iAtY);

/**
 * @brief Tells if the snake head is located on a part of himself.
 * 		  In other words, tells if the snake bites himself! 
 * 
 * @param pSnake pointer to the snake to check.
 * 
 * @return int 	 a non nul value in case of bite, zero otherwise.
 */
int SnakeHasBittenHimself(const t_snake*pSnake);


#endif /* SNAKE_H_ */
