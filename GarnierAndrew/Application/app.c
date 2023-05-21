/**
 * @file app.c
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
#include <time.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "appDefines.h"
#include "app.h"

#include <scene.h>


#define mBitsSet(f,m)       ((f)|=(m))
#define mBitsClr(f,m)       ((f)&=(~(m)))
#define mBitsTgl(f,m)       ((f)^=(m))
#define mBitsMsk(f,m)       ((f)& (m))
#define mIsBitsSet(f,m)     (((f)&(m))==(m))
#define mIsBitsClr(f,m)     (((~(f))&(m))==(m))

/**
 * @brief application status masks definition.
 */
typedef enum s_statusMasks{
    ST_ALL_CLEARED          = 0x00000000,
    ST_APP_INIT_FAILED      = 0x80000000,
    ST_APP_SDL_INITIATED    = 0x00000001,
    ST_APP_TTF_INITIATED    = 0x00000002,
    ST_APP_PAUSED           = 0x00000100,
    ST_APP_GAME_OVER        = 0x00000200,
}t_status;

/**
 * @brief the application structure definition
 */
	struct s_app{
		t_status        m_uStatus;		/* status flags of application			*/
		SDL_Window  *   m_pWindow;		/* the main window structure pointer 	*/
		SDL_Renderer*   m_pRenderer;	/* the main window graphical renderer	*/
		SDL_TimerID     m_timerID;		/* the main timer ID					*/
        TTF_Font    *   m_pFont;        /* the main font                        */
		/*--------------------------*/
		t_scene*        m_pScene;		/* the scene entity pointer				*/
	};


/**
 * @brief Application timer callback function.
 *        Performs annimation of the scene.
 * 
 * @param interval period in ms.
 * @param pApp this current application.
 * @return Uint32 next period in ms.
 */
Uint32 _AppTimerCallback(Uint32 interval, t_app*pApp);


/**
 * @brief The application "constructor".
 *        Performs memory allocation, and
 *        components initialization.
 * 
 * @return t_app* pointer to the newly created application.
 */
t_app*AppNew(void){

	srand((unsigned int)time(NULL));

	t_app*pApp=(t_app*)malloc(sizeof(t_app));
    assert(pApp);
    *pApp=(t_app){
        .m_uStatus = ST_ALL_CLEARED,
    };
    
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        fprintf(stderr,"Failed to initiate SDL: %s\n", SDL_GetError());
        mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
        return pApp;
    }
    mBitsSet(pApp->m_uStatus, ST_APP_SDL_INITIATED);

    pApp->m_pWindow=SDL_CreateWindow(
        APP_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        APP_WINDOW_WIDTH,
        APP_WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if(pApp->m_pWindow==NULL){
        fprintf(stderr,"Failed to create window: %s\n", SDL_GetError());
        mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
        return pApp;
    }

    pApp->m_pRenderer=SDL_CreateRenderer(pApp->m_pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(pApp->m_pRenderer==NULL){
        fprintf(stderr,"Failed to create accelerated renderer: %s\n", SDL_GetError());
        pApp->m_pRenderer=SDL_CreateRenderer(pApp->m_pWindow, -1, SDL_RENDERER_SOFTWARE);
        if(pApp->m_pRenderer==NULL){
            fprintf(stderr,"Failed to create software renderer: %s\n", SDL_GetError());
            mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
            return pApp;
        }
        fprintf(stderr,"Created software renderer instead!\n");
    }

    if(TTF_Init()!=0){
        fprintf(stderr,"Failed to initiate TTF: %s\n", SDL_GetError());
        mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
        return pApp;
    }
    mBitsSet(pApp->m_uStatus, ST_APP_TTF_INITIATED);

    pApp->m_pFont=TTF_OpenFont(APP_MAIN_FONT_FILE_NAME, APP_MAIN_FONT_SIZE);
    if(pApp->m_pFont==NULL){
        fprintf(stderr,"Failed to open font: %s\n", SDL_GetError());
        mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
        return pApp;
    }

    pApp->m_pScene = SceneNew(
        APP_WINDOW_WIDTH,
        APP_WINDOW_HEIGHT,
        pApp->m_pRenderer,
        pApp->m_pFont);

    return pApp;
}

/**
 * @brief The application "destructor".
 *        Performs memory de-allocation, and
 *        freeing all resources.
 * 
 * @param pApp pointer to the application to destroy.
 * @return t_app* NULL. 
 */
t_app*AppDel(t_app*pApp){
    assert(pApp);

    if(pApp->m_timerID) SDL_RemoveTimer(pApp->m_timerID);

    SceneDel(pApp->m_pScene);

    if(pApp->m_pRenderer) SDL_DestroyRenderer(pApp->m_pRenderer);
    if(pApp->m_pWindow) SDL_DestroyWindow(pApp->m_pWindow);
    if(pApp->m_pFont) TTF_CloseFont(pApp->m_pFont);

    if(mIsBitsSet(pApp->m_uStatus, ST_APP_TTF_INITIATED)) TTF_Quit();

    if(mIsBitsSet(pApp->m_uStatus, ST_APP_SDL_INITIATED)) SDL_Quit();
    free(pApp);
    return NULL;
}

/**
 * @brief The main application loop.
 *        Processes all events from OS.
 * 
 * @param pApp pointer to the application to run.
 * @return int EXIT_SUCCES in case of a normal issue,
 *             EXIT_FAILURE otherwise.
 */
int AppRun(t_app*pApp){
    assert(pApp);
    if(mIsBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED)) return EXIT_FAILURE;

    pApp->m_timerID=SDL_AddTimer((Uint32)(1000.0/DEF_FPS), (SDL_TimerCallback)_AppTimerCallback, pApp);

    int iQuit=0;
    SDL_Event event;

    while(!iQuit){
        while(SDL_PollEvent(&event)){
            switch (event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    event.type=SDL_QUIT;
                    SDL_PushEvent(&event);
                    break;
                case SDLK_p:
                    mBitsTgl(pApp->m_uStatus, ST_APP_PAUSED);
                    break;
                default:
                    SceneKeydown(pApp->m_pScene, &event.key);
                    break;
                }
                break;
            case SDL_KEYUP:
                SceneKeyup(pApp->m_pScene, &event.key);
                break;
            case SDL_QUIT:
                iQuit=1;
                break;
            default:
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}


/**
 * @brief Application timer callback function.
 *        Performs annimation of the scene.
 * 
 * @param interval period in ms.
 * @param pApp this current application.
 * @return Uint32 next period in ms.
 */
Uint32 _AppTimerCallback(Uint32 interval, t_app*pApp){
    if(mIsBitsSet(pApp->m_uStatus, ST_APP_PAUSED)) return interval;

    if(mIsBitsClr(pApp->m_uStatus, ST_APP_GAME_OVER)){
        if(SceneAnimate(pApp->m_pScene)==NULL) mBitsSet(pApp->m_uStatus, ST_APP_GAME_OVER);
    }
    SceneDraw(pApp->m_pScene);

    return interval;
}
