/**
 * @file scene.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-14
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

#include <snake.h>

#include <snakeDefines.h>
#include "scene.h"
#include "sceneDefines.h"

/**
 * @brief A service for inflating a rectangle structure.
 *        The inflated rectangle has the same center as
 *        the original one.
 * 
 * @param pRect a pointer to the original rectangle of which a copy will be inflated.
 * @param iFlat the inflat relative value : > 0 grows up the rectangle
 *                                          < 0 shrinks down the rectangle
 * @return SDL_Rect* a pointer to a static local rectangle structure
 *                   containing the inflated copy.
 * 
 * @remark The original rectangle is not modified by this function.
 */
SDL_Rect*_InflatRect(const SDL_Rect*pRect, int iFlat){
    static SDL_Rect r;
    r=(SDL_Rect){
        .x=pRect->x-iFlat,
        .y=pRect->y-iFlat,
        .w=pRect->w+2*iFlat,
        .h=pRect->h+2*iFlat
    };
    return &r;
}

/**
 * @brief the scene structure definition.
 */
	struct s_scene{
		SDL_Rect         m_frameArea;		/* for representing the scene frame delimiting area */
		SDL_Rect         m_gameArea;		/* for representing the effective game area			*/
		SDL_Renderer    *m_pRenderer;		/* window scene graphical renderer 					*/
		SDL_Color        m_colorBkgnd;		/* RGBA background color							*/
		SDL_Color        m_colorFrame;		/* RGBA scene frame delimiter color					*/
        TTF_Font        *m_pFont;           /* current font for textual items                   */
		/*-----------------------------*/
		t_snake         *m_pSnake;			/* the snake entity pointer							*/
		SDL_Point        m_ptFood;			/* the tabular coordinates of snake food			*/
		Uint32           m_foodTimer;		/* the timer food generator							*/
		Uint32			 m_score;			/* the player game score							*/
		Uint32			 m_elapsedTime;		/* the game elapsed time in seconds					*/
	};

/**
 * @brief The scene "constructor".
 *        Performs memory allocation, and
 *        components initialization.
 * 
 * @param iWidth horizontal size of the scene.
 * @param iHeight vertical size of the scene.
 * @param pRenderer pointer to the window renderer for graphical drawings.
 * @param colorBkgnd scene RGBA background color.
 * @return t_scene* pointer to the newly created scene.
 */
t_scene*SceneNew(int iWidth, int iHeight, SDL_Renderer*pRenderer, TTF_Font*pFont){
    t_scene*pScene=(t_scene*)malloc(sizeof(t_scene));
    assert(pScene);
    *pScene=(t_scene){
        .m_frameArea    = (SDL_Rect){
                              .x = 0,
                              .y = 0,
                              .w = iWidth,
                              .h = iHeight  
                            },
        .m_gameArea    = (SDL_Rect){
                              .x = SCENE_PADDING_HRZ+SCENE_FRAME_WIDTH,
                              .y = SCENE_PADDING_TOP+SCENE_FRAME_WIDTH,
                              .w = iWidth-(2*SCENE_PADDING_HRZ+2*SCENE_FRAME_WIDTH),
                              .h = iHeight-(SCENE_PADDING_TOP+SCENE_PADDING_BOT+2*SCENE_FRAME_WIDTH)  
                            },
        .m_pRenderer    = pRenderer,
        .m_colorBkgnd   = SCENE_COLOR_BKGND,
        .m_colorFrame   = SCENE_COLOR_FRAME,
        .m_pFont        = pFont,

        /**
         * @todo create a new snake entity  
         */
        .m_pSnake       = SnakeNew(3, 0, 0, DIR_RIGHT, &SCENE_COLOR_SNAKE),

        .m_ptFood.x     = -1,

        /**
         * @todo random a food timer value  
         */
        .m_foodTimer    =  rand()%(SCENE_FOOD_TIMER_MAX-SCENE_FOOD_TIMER_MIN +1)+SCENE_FOOD_TIMER_MIN ,
    };
    return pScene;
}

/**
 * @brief The scene "destructor".
 *        Performs memory de-allocation, and
 *        freeing all resources.
 * 
 * @param pScene pointer to the scene to destroy. 
 * @return t_scene* NULL;
 */
t_scene*SceneDel(t_scene*pScene){
    assert(pScene);

    /**
     * @todo destroy the snake entity
     */

    pScene->m_pSnake = SnakeDel(pScene->m_pSnake);

    free(pScene);
    return NULL;
}

/**
 * @brief Performs all drawings, scene and all contained graphical objects.
 * 
 * @param pScene pointer to the scene to draw.
 * @return t_scene* pointer to the scene himself.
 */
t_scene*SceneDraw(t_scene*pScene){
    assert(pScene);

    SDL_SetRenderDrawColor(
        pScene->m_pRenderer, 
        pScene->m_colorBkgnd.r,
        pScene->m_colorBkgnd.g,
        pScene->m_colorBkgnd.b,
        pScene->m_colorBkgnd.a);
    SDL_RenderClear(pScene->m_pRenderer);


    /* Drawing food, if exists! **************************************************************************/
    if(pScene->m_ptFood.x!=-1){
        SDL_SetRenderDrawColor(pScene->m_pRenderer, 230,20,20,255);
        SDL_RenderFillRect(
            pScene->m_pRenderer,
            &(SDL_Rect){
                .x = pScene->m_gameArea.x + pScene->m_ptFood.x*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)+(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)*.15,
                .y = pScene->m_gameArea.y + pScene->m_ptFood.y*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)+(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)*.15,
                .w = (SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)*.70,
                .h = (SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)*.70
            });
    }

    /* Ask for drawing the snake entity ************************************************/
    /**
     * @todo call the snake entity drawing function
     */

    SnakeDraw(pScene->m_pSnake, pScene->m_pRenderer, &pScene->m_gameArea);


    /* Drawing the frame border ********************************************************/
    SDL_SetRenderDrawColor(
        pScene->m_pRenderer,
        pScene->m_colorFrame.r,
        pScene->m_colorFrame.g,
        pScene->m_colorFrame.b,
        pScene->m_colorFrame.a
    );
    SDL_RenderFillRect(
        pScene->m_pRenderer,
        &(SDL_Rect){
            pScene->m_frameArea.x+SCENE_PADDING_HRZ,
            pScene->m_frameArea.y+SCENE_PADDING_TOP,
            pScene->m_frameArea.w-2*SCENE_PADDING_HRZ,
            SCENE_FRAME_WIDTH
        });
    SDL_RenderFillRect(
        pScene->m_pRenderer,
        &(SDL_Rect){
            pScene->m_frameArea.x+SCENE_PADDING_HRZ,
            pScene->m_frameArea.y+SCENE_PADDING_TOP+SCENE_FRAME_WIDTH,
            SCENE_FRAME_WIDTH,
            pScene->m_frameArea.h-(SCENE_PADDING_TOP+SCENE_PADDING_BOT+2*SCENE_FRAME_WIDTH)
        });
    SDL_RenderFillRect(
        pScene->m_pRenderer,
        &(SDL_Rect){
            pScene->m_frameArea.x+pScene->m_frameArea.w-SCENE_PADDING_HRZ-SCENE_FRAME_WIDTH,
            pScene->m_frameArea.y+SCENE_PADDING_TOP+SCENE_FRAME_WIDTH,
            SCENE_FRAME_WIDTH,
            pScene->m_frameArea.h-(SCENE_PADDING_TOP+SCENE_PADDING_BOT+2*SCENE_FRAME_WIDTH)
        });
    SDL_RenderFillRect(
        pScene->m_pRenderer,
        &(SDL_Rect){
            pScene->m_frameArea.x+SCENE_PADDING_HRZ,
            pScene->m_frameArea.y+pScene->m_frameArea.h-(SCENE_FRAME_WIDTH+SCENE_PADDING_BOT),
            pScene->m_frameArea.w-2*SCENE_PADDING_HRZ,
            SCENE_FRAME_WIDTH
        });
    SDL_SetRenderDrawColor(
        pScene->m_pRenderer,
        255,0,0,255
    );
    SDL_RenderDrawRect(pScene->m_pRenderer, &pScene->m_gameArea);


    /* Drawing the textual items **********************************************************************/
    SDL_Surface*pSurf;
    SDL_Texture*pText;
    char buffer[64];
    /* Processing elapsed time -------------------------------------------------*/
    sprintf(buffer,"%06d", pScene->m_elapsedTime);
    pSurf=TTF_RenderText_Blended(pScene->m_pFont, buffer, SCENE_COLOR_TEXT);
    pText=SDL_CreateTextureFromSurface(pScene->m_pRenderer, pSurf);
    SDL_RenderCopy(
        pScene->m_pRenderer, 
        pText, 
        NULL, 
        &(SDL_Rect){
            .x = pScene->m_frameArea.x+20,
            .y = pScene->m_frameArea.y+10,
            pSurf->w,
            pSurf->h});
    SDL_FreeSurface(pSurf);
    SDL_DestroyTexture(pText);
    /* Processing player game score -------------------------------------------*/
    sprintf(buffer,"%06d", pScene->m_score);
    pSurf=TTF_RenderText_Blended(pScene->m_pFont, buffer, SCENE_COLOR_TEXT);
    pText=SDL_CreateTextureFromSurface(pScene->m_pRenderer, pSurf);
    SDL_RenderCopy(
        pScene->m_pRenderer, 
        pText, 
        NULL, 
        &(SDL_Rect){
            .x = pScene->m_frameArea.x+pScene->m_frameArea.w-20-pSurf->w,
            .y = pScene->m_frameArea.y+10,
            pSurf->w,
            pSurf->h});
    SDL_FreeSurface(pSurf);
    SDL_DestroyTexture(pText);

    /* Rendering the final scene ***********************************************************************/
    SDL_RenderPresent(pScene->m_pRenderer);
    return pScene;
}

/**
 * @brief Performs all animations, scene and all contained graphical objects.
 * 
 * @param pScene pointer to the scene to animate.
 * @return t_scene* pointer to the scene himself.
 */
t_scene*SceneAnimate(t_scene*pScene){
    assert(pScene);

    /* Ask for moving the snake entity ************************************************/
    /**
     * @todo call the snake entity moving function
     */

    SnakeMove(pScene->m_pSnake, &pScene->m_gameArea);

    /* Taking care of fooding the snake entity *****************************************/
    /**
     * @todo checking if snake entity is on food location, and taking actions if it is. 
     */

    if(SnakeIsHeadAt(pScene->m_pSnake, pScene->m_ptFood.x, pScene->m_ptFood.y)){
    	SnakeGrowup(pScene->m_pSnake);
    	pScene->m_ptFood.x = -1;
    	pScene->m_score++;
    }


    /* Taking care of food generation process ******************************************/
    /**
     * @todo checking if there is no food in scene and taking actions it is. 
     */
    if(pScene->m_ptFood.x == -1){
    	pScene->m_foodTimer =  rand()%(SCENE_FOOD_TIMER_MAX-SCENE_FOOD_TIMER_MIN +1) + SCENE_FOOD_TIMER_MIN;
    	pScene->m_ptFood.x = rand()%(pScene->m_gameArea.w/ (SNAKE_SECTION_SIZE+ SNAKE_SECTION_SPACING)-pScene->m_gameArea.x/ (SNAKE_SECTION_SIZE+ SNAKE_SECTION_SPACING))+pScene->m_gameArea.x/ (SNAKE_SECTION_SIZE+ SNAKE_SECTION_SPACING);
    	pScene->m_ptFood.y =  rand()%(pScene->m_gameArea.h/ (SNAKE_SECTION_SIZE+ SNAKE_SECTION_SPACING)-pScene->m_gameArea.y/ (SNAKE_SECTION_SIZE+ SNAKE_SECTION_SPACING))+pScene->m_gameArea.y/ (SNAKE_SECTION_SIZE+ SNAKE_SECTION_SPACING);
    	;
    }
    else{
    	pScene->m_foodTimer--;
    }


    /* Taking care of snake biting himself *********************************************/
    /**
     * @todo checking if there the snake is biting himself and return NULL if it is. 
     */
   if(SnakeHasBittenHimself(pScene->m_pSnake)){
	    return NULL;
   }


    return pScene;
}

/**
 * @brief Performs all keydown events from application.
 * 
 * @param pScene pointer to the scene entity to handle the event.
 * @param pEvent pointer to the keyboard event.
 * 
 * @return t_scene* pointer to the current scene entity.
 */
t_scene*SceneKeydown(t_scene*pScene, SDL_KeyboardEvent*pEvent){
    assert(pScene);
    assert(pEvent);
    switch (pEvent->keysym.sym){
    case SDLK_RIGHT:
    case SDLK_LEFT:
    case SDLK_DOWN:
    case SDLK_UP:
        if(pScene->m_pSnake) SnakeChangeDirection(pScene->m_pSnake, pEvent->keysym.sym-SDLK_RIGHT);
        break;
    case SDLK_SPACE:
        break;
    default:
        break;
    }
    return pScene;
}

/**
 * @brief Performs all keyup events from application.
 * 
 * @param pScene pointer to the scene entity to handle the event.
 * @param pEvent pointer to the keyboard event.
 * 
 * @return t_scene* pointer to the current scene entity.
 */
t_scene*SceneKeyup(t_scene*pScene, SDL_KeyboardEvent*pEvent){
    assert(pScene);
    assert(pEvent);
    switch (pEvent->keysym.sym){
    case SDLK_RIGHT:
    case SDLK_LEFT:
    case SDLK_DOWN:
    case SDLK_UP:
        break;
    case SDLK_SPACE:
        break;
    default:
        break;
    }
    return pScene;
}
