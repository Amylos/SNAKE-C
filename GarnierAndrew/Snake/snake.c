/**
 * @file snake.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "snakeDefines.h"
#include "snake.h"

/**
 * @brief The section structure definition.
 *        This entity composes any snake entity in a
 *        double chained list.
 */
typedef struct s_section{
    int         		m_iPosX;		/* section x coordinate 		*/
    int         		m_iPosY;		/* section y coordinate 		*/
    t_direction 		m_direction;	/* section moving direction  	*/
    struct s_section* 	m_pNext;		/* pointer to next section 		*/
    struct s_section* 	m_pPrev;		/* pointer to previous section 	*/
}t_section;

/**
 * @brief Create a new section entity and performs its initialization with
 *        passed parameters. In the same time, performs the new section 
 *        chaining to the existing chained list.
 * 
 * @param pPrev     pointer to the previous section to this created one.
 * @param pNext     pointer to the next section to this created one.
 * @param iPosX     tabular x coordinate of the newly created section.
 * @param iPosY     tabular y coordinate of the newly created section.
 * @param direction direction adopted by the newly created section.
 * 
 * @return t_section* the pointer to the newly created section.
 */
t_section*SectionNew(t_section*pPrev, t_section*pNext, int iPosX, int iPosY, t_direction direction){
    t_section*pSection=(t_section*)malloc(sizeof(t_section));
    assert(pSection);
    /**
     * @todo complete the initialization code
     */
    *pSection = (t_section){
    	.m_iPosX = iPosX,
    	.m_iPosY = iPosY,
		.m_direction = direction,
		.m_pNext = pNext,
		.m_pPrev = pPrev,
    };

    if(pPrev) pPrev->m_pNext = pSection;
    if(pNext) pNext->m_pPrev = pSection;

    return pSection;
}

/**
 * @brief Destroy the section passed by parameter.
 *        Performs re-chaining and release all memory resources.
 * 
 * @param pSection  pointer to the section to destroy.
 * 
 * @return t_section* the pointer to the next section to this was destroyed.
 */
t_section*SectionDelReturnNext(t_section*pSection){
    assert(pSection);
    t_section*pNext=pSection->m_pNext;
    /**
     * @todo complete the destruction code
     */

    if(pSection->m_pPrev) pSection->m_pPrev->m_pNext = pSection->m_pNext;
    if(pSection->m_pNext) pSection->m_pNext->m_pPrev = pSection->m_pPrev;

    free(pSection);

    return pNext;
}

/**
 * @brief The snake structure definition.
 *        All snake entity conforms to this description.
 */
struct s_snake {
    t_section * m_pHead;		/* pointer to the snake head section  */
    t_section * m_pTail;		/* pointer to the snake tail section  */
    SDL_Color   m_color;		/* RGBA snake color					  */
    size_t      m_szLength;		/* snake length in number of sections */
};

/**
 * @brief Performs "a snake grown up" by appending a new section to the
 *        snake tail. 
 *        The new section coordinates derive from the tail ones according
 *        to its own direction.
 * 
 * @param pSnake pointer to the snake entity to be grown up.
 * 
 * @return t_snake*	pointer to the current snake entity.  
 */
t_snake*_SnakePushback(t_snake*pSnake){
    assert(pSnake);
    int iPosX=-1, iPosY=-1;
    switch(pSnake->m_pTail->m_direction){
    case DIR_RIGHT:
        /**
         * @todo complete the case code
         */

    	iPosX = pSnake->m_pTail->m_iPosX-1;
    	iPosY = pSnake->m_pTail->m_iPosY;
        break;
    case DIR_LEFT:
        /**
         * @todo complete the case code
         */
    	iPosX = pSnake->m_pTail->m_iPosX+1;
    	iPosY = pSnake->m_pTail->m_iPosY;

        break;
    case DIR_UP:
        /**
         * @todo complete the case code
         */
    	iPosX = pSnake->m_pTail->m_iPosX;
    	iPosY = pSnake->m_pTail->m_iPosY+1;

        break;
    case DIR_DOWN:
        /**
         * @todo complete the case code
         */
    	iPosX = pSnake->m_pTail->m_iPosX;
    	iPosY = pSnake->m_pTail->m_iPosY-1;

        break;
    default:
        assert(NULL);
        break;
    }

    pSnake->m_pTail=SectionNew(pSnake->m_pTail, NULL, iPosX, iPosY, pSnake->m_pTail->m_direction);
    pSnake->m_szLength++;
    
    return pSnake;
}

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
t_snake*SnakeNew(size_t szNbSections, int iHeadPosX, int iHeadPosY, t_direction direction, const SDL_Color*pColor){
    t_snake*pSnake=(t_snake*)malloc(sizeof(t_snake));
    assert(pSnake);
    /**
     * @todo complete the initialization code
     */

    *pSnake = (t_snake){
    	.m_szLength = 1,
    	.m_color = *pColor,
    };

    pSnake->m_pHead = SectionNew(NULL, NULL, iHeadPosX, iHeadPosY, direction);
    pSnake->m_pTail = pSnake->m_pHead;

    for(size_t k =1;k<szNbSections;k++){
    	SnakeGrowup(pSnake);
    }

    return pSnake;
}

/**
 * @brief The snake entity destructor.
 *        Performs memory de-allocation, and
 *        freeing all resources.
 * 
 * @param pSnake 	pointer to the snake to destroy.
 * 
 * @return t_snake* NULL pointer.
 */
t_snake*SnakeDel(t_snake*pSnake){
    assert(pSnake);
    /**
     * @todo complete the destruction code
     */
    while(pSnake->m_pHead){
    	pSnake->m_pHead =  SectionDelReturnNext(pSnake->m_pHead);
    	pSnake->m_szLength--;
    }

    assert(pSnake->m_szLength==0);
    free(pSnake);
    return NULL;
}

/**
 * @brief Performs snake drawing by drawing all snake sections.
 * 
 * @param pSnake 	pointer to the snake to draw.
 * @param pRenderer pointer to the graphical renderer to draw in.
 * @param pArea 	pointer to the drawing area.
 * 
 * @return t_snake*	pointer to the current snake entity. 
 */
t_snake*SnakeDraw(t_snake*pSnake, SDL_Renderer*pRenderer, const SDL_Rect*pArea){
    assert(pSnake);
    SDL_SetRenderDrawColor(
        pRenderer, 
        pSnake->m_color.r,
        pSnake->m_color.g,
        pSnake->m_color.b,
        pSnake->m_color.a);
    
    t_section*pParse=pSnake->m_pHead;
    
    while(pParse){
        /**
         * @todo complete the drawing code
         */
    	SDL_RenderFillRect(pRenderer, &(SDL_Rect){
										pArea->x + pParse->m_iPosX*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING),
										pArea->y + pParse->m_iPosY*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING),
										(SNAKE_SECTION_SIZE),
										(SNAKE_SECTION_SIZE)});


        pParse=pParse->m_pNext;
    }
    return pSnake;
}

/**
 * @brief Performs snake movements by perfoming movements of all snake sections.
 * 
 * @param pSnake 	pointer to the snake to move.
 * @param pArea 	pointer to the moving area.
 * 
 * @return t_snake*	pointer to the current snake entity. 
 */
t_snake*SnakeMove(t_snake*pSnake, const SDL_Rect*pArea){
    assert(pSnake);
    t_section*pParse=pSnake->m_pHead;
    while(pParse){
        switch (pParse->m_direction){
        case DIR_RIGHT:
            /**
             * @todo complete the case code
             */
        	pParse->m_iPosX ++;
        	if(pParse->m_iPosX>=pArea->w/(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)){
        		pParse->m_iPosX =0;
        	}

            break;
        case DIR_LEFT:
            /**
             * @todo complete the case code
             */

        	pParse->m_iPosX--;
        	if(pParse->m_iPosX<=0){
				pParse->m_iPosX =(pArea->w)/(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING);
        	}

            break;
        case DIR_DOWN:
            /**
             * @todo complete the case code
             */
        	pParse->m_iPosY++;
        	if(pParse->m_iPosY>=pArea->h/(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)){
				pParse->m_iPosY =0;
			}

            break;
        case DIR_UP:
            /**
             * @todo complete the case code
             */
        	pParse->m_iPosY--;
        	if(pParse->m_iPosY<=0){
				pParse->m_iPosY =(pArea->h)/(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING);
			}

            break;
        default:
            assert(NULL);
            break;
        }
        pParse=pParse->m_pNext;
    }

    pParse=pSnake->m_pTail;
    while(pParse){
        /**
         * @todo complete the while code
         */
    	if(pParse->m_pPrev) pParse->m_direction = pParse->m_pPrev->m_direction;

        pParse=pParse->m_pPrev;
    }
    return pSnake;
}

/**
 * @brief Performs snake growing up by adding a section to the tail.
 * 
 * @param pSnake 	pointer to the snake to grow up.
 * 
 * @return t_snake*	pointer to the current snake entity. 
 */
t_snake*SnakeGrowup(t_snake*pSnake){
    assert(pSnake);
    /**
     * @todo complete the code
     */
    for(size_t k = 0 ; k<5;k++){
    	_SnakePushback(pSnake);
    }


    return pSnake;
}

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
t_snake*SnakeChangeDirection(t_snake*pSnake, t_direction direction){
    assert(pSnake);
    switch (direction) {
    case DIR_RIGHT:
        /**
         * @todo complete the case code
         */
    	if(pSnake->m_pHead->m_direction == DIR_LEFT || pSnake->m_pHead->m_direction == DIR_RIGHT)	return pSnake;

        break;
    case DIR_LEFT:
        /**
         * @todo complete the case code
         */
    	if(pSnake->m_pHead->m_direction == DIR_RIGHT || pSnake->m_pHead->m_direction == DIR_LEFT)	return pSnake;
        break;
    case DIR_UP:
        /**
         * @todo complete the case code
         */
    	if(pSnake->m_pHead->m_direction == DIR_DOWN || pSnake->m_pHead->m_direction == DIR_UP)	return pSnake;

        break;
    case DIR_DOWN:
        /**
         * @todo complete the case code
         */
    	if(pSnake->m_pHead->m_direction == DIR_UP || pSnake->m_pHead->m_direction == DIR_DOWN)	return pSnake;

        break;
    default:
        assert(NULL);
        break;
    }
    pSnake->m_pHead->m_direction = direction;
    return pSnake;
}

/**
 * @brief Tells if the snake head is located at passed coordinates prameters.
 * 
 * @param pSnake pointer to the snake to check.
 * @param iAtX 	 x coordinate to check.
 * @param iAtY 	 y coordinate to check.
 * 
 * @return int   a non nul value in case of match, zero otherwise.
 */
int SnakeIsHeadAt(const t_snake*pSnake, int iAtX, int iAtY){
    assert(pSnake);
    /**
     * @todo complete the code
     */

    if(pSnake->m_pHead->m_iPosX == iAtX && pSnake->m_pHead->m_iPosY == iAtY) return 1;

    return 0;
}

/**
 * @brief Tells if the snake head is located on a part of himself.
 * 		  In other words, tells if the snake bites himself! 
 * 
 * @param pSnake pointer to the snake to check.
 * 
 * @return int 	 a non nul value in case of bite, zero otherwise.
 */
int SnakeHasBittenHimself(const t_snake*pSnake){
    assert(pSnake);
    t_section*pScan=pSnake->m_pHead;
    /**
     * @todo complete the code
     */

    while(pScan){
    	if(pSnake->m_pHead->m_iPosX == pScan->m_iPosX && pSnake->m_pHead->m_iPosY == pScan->m_iPosY) return 1;
    	pScan = pScan->m_pNext;
    }

    return 0;
}
