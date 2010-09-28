// Copyright (C) 2009 =F=-RedFox-
//

#include "../precompiled.h"
#pragma hdrstop

// FORMIDO BEGIN
#ifdef RF_DIASPORA_BUILD

#include "../Game_local.h"

#include "DiasporaThread.h"

rfDiasporaThread diasporaThreadLocal;
rfDiasporaThread* diasporaThread = &diasporaThreadLocal;

/*
=====================
rfDiasporaThread::rfDiasporaThread
=====================
*/
rfDiasporaThread::rfDiasporaThread() :
        thread( NULL ),
        isWaiting( false ),
        isActive( false ) {
}

/*
=====================
rfDiasporaThread::~rfDiasporaThread
=====================
*/
rfDiasporaThread::~rfDiasporaThread() {
}

/*
=====================
rfDiasporaThread::StartThread
=====================
*/
void rfDiasporaThread::StartThread() {
        if ( thread != NULL ) {
                return;
        }
#ifdef _XENON
        thread = new sdThread( this, THREAD_NORMAL, XENON_STACKSIZE_BOT );
#else
        thread = new sdThread( this, THREAD_LOWEST );
#endif
        thread->SetName( "DiasporaThread" );

#ifdef _XENON
        thread->SetProcessor( XENON_THREADCORE_BOT );
#endif

        if ( !thread->Start() ) {
                common->Error( "rfDiasporaThread::StartThread : failed to start thread" );
        }
}

/*
=====================
rfDiasporaThread::StopThread
=====================
*/
void rfDiasporaThread::StopThread() {
        if ( thread != NULL ) {
                thread->Stop();
                thread->Join();
                thread->Destroy();
                thread = NULL;
        }
}

/*
=====================
rfDiasporaThread::Run
=====================
*/
unsigned int rfDiasporaThread::Run( void *parm ) {
        isActive = true;

        gameLocal.clip.AllocThread();

        int i;
        idPlayer* player;

        while ( !Terminating() ) {
                // FIXME: RedFox: Do we need this? Game thread never waits for diaspora thread anyway.
                //SignalGameThread();

                isWaiting = true;
                WaitForGameThread();
                isWaiting = false;

                if ( !Terminating() ) {

                        for( i = 0; i < MAX_CLIENTS; i++ ) {
                                player = gameLocal.GetClient( i );

                                if ( player == NULL ) {
                                        continue;
                                }

                                if ( player->WantsHop() && !player->IsHopping() ) {
                                        player->HopServer();
                                }
                        }
                }
        }

        gameLocal.clip.FreeThread();

        isActive = false;

        return 0;
}

/*
=====================
rfDiasporaThread::Stop
=====================
*/
void rfDiasporaThread::Stop() {
        sdThreadProcess::Stop();
        // need to signal the thread so it wakes up and terminates
        gameSignal.Set();
        diasporaSignal.Set();
}

#endif /* RF_DIASPORA_BUILD */
// FORMIDO END
