#ifndef __WW_WEATHER_H__
#define __WW_WEATHER_H__


#define MAX_WEATHER_PARTICLES			256

class WeatherManager
{
public:
	WeatherManager();
	~WeatherManager();

	int				Process( float flTime );
	void			Create( void );
	void			Killed( WWParticle * pPart );

private:
	float			m_flNextCheck;
	int				m_iTotal;
	bool			m_bInit;
};


extern WeatherManager * g_pWeatherManager;


#endif // __WW_WEATHER_H__
