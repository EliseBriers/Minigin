#include "MiniginPCH.h"
#include "UpdateInfo.h"
#include "ISoundSystem.h"

dae::UpdateInfo::UpdateInfo( float dt, ISoundSystem& soundSystem )
	: m_DeltaTime{ dt }
	, m_SoundSystem{ soundSystem }
{
}

void dae::UpdateInfo::PushSound( size_t soundId, float volume ) const
{
	m_SoundSystem.PushSound( soundId, volume );
}

float dae::UpdateInfo::GetDeltaTime( ) const
{
	return m_DeltaTime;
}
