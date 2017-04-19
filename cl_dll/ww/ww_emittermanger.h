#ifndef __WW_EMITTERMANGER_H__
#define __WW_EMITTERMANGER_H__


class WWEmitterManager
{
public:
	WWEmitterManager();
	~WWEmitterManager();

	void			Clear( void );
	WWEmitter *		Allocate( void );

	void			Process( float flTime );

	WWEmitter *		Head( void );
	void			SetHead( WWEmitter * pPart );

	WWEmitter *		Tail( void );
	void			SetTail( WWEmitter * pPart );

private:
	WWEmitter *		m_pHead;
	WWEmitter *		m_pTail;
};


extern WWEmitterManager * g_pEmitterManager;


#endif // __WW_PARTICLEMANGER_H__