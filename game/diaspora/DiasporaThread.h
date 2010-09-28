// Copyright (C) 2009 =F=-RedFox-
//

// FORMIDO BEGIN
#ifdef RF_DIASPORA_BUILD

#ifndef __DIASPORATHREAD_H__
#define __DIASPORATHREAD_H__

class rfDiasporaThread : public sdThreadProcess {
public:
                                                        rfDiasporaThread();
                                                        ~rfDiasporaThread();

        void					StartThread();
        void					StopThread();

        virtual unsigned int                    Run( void* parm );
        virtual void                            Stop();

        void					WaitForGameThread() { gameSignal.SignalAndWait( diasporaSignal ); }
        void					SignalGameThread() { diasporaSignal.Set(); }

        void					WaitForDiasporaThread() { diasporaSignal.Wait(); }
        void					SignalDiasporaThread() { gameSignal.Set(); }

        void					Lock() { lock.Acquire(); }
        void					UnLock() { lock.Release(); }

        bool					IsActive() const { return isActive; }
        bool					IsWaiting() const { return isWaiting; }

private:
        sdThread *				thread;
        sdSignal				gameSignal;
        sdSignal				diasporaSignal;
        sdLock					lock;

        bool					isWaiting;
        bool					isActive;
};

extern rfDiasporaThread *		diasporaThread;

#endif /* !__DIASPORATHREAD_H__ */

#endif /* RF_DIASPORA_BUILD */
// FORMIDO END
