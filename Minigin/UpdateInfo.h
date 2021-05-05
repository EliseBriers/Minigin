#pragma once

namespace dae
{
	class ISoundSystem;

	class UpdateInfo
	{
	public:
		UpdateInfo( float dt, ISoundSystem& soundSystem );

		void PushSound( uint16_t soundId, float volume ) const;
		float GetDeltaTime( ) const;
	private:
		float m_DeltaTime;
		ISoundSystem& m_SoundSystem;
	};
}
