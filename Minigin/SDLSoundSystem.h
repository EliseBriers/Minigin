#pragma once
#include "ISoundSystem.h"
#include "ManagerMap.h"
#include <deque>
#include <mutex>

struct Mix_Chunk;

namespace dae
{
	class SDLSoundSystem final : public ISoundSystem
	{
	private:
		struct SDLSound
		{
			SDLSound( );
			SDLSound( uint16_t id, const std::string& fileName, Mix_Chunk* pChunk );

			uint16_t Id;
			std::string Name;
			Mix_Chunk* pSound;
		};

		struct SoundEvent
		{
			SoundEvent( );
			SoundEvent( uint16_t id, float volume );;

			uint16_t Id;
			float Volume;
		};

	public:
		SDLSoundSystem( );

		void PushSound( uint16_t soundId, float volume ) override;
		void ProcessSounds( ) override;
		uint16_t GetSound( const std::string& fileName ) override;
		void Init( ) override;

		// Rule of 5
		~SDLSoundSystem( ) = default;
		SDLSoundSystem( const SDLSoundSystem& other ) = delete;
		SDLSoundSystem( SDLSoundSystem&& other ) noexcept = delete;
		SDLSoundSystem& operator=( const SDLSoundSystem& other ) = delete;
		SDLSoundSystem& operator=( SDLSoundSystem&& other ) noexcept = delete;

	private:
		Mix_Chunk* GetChunk( uint16_t id );
		std::vector<SDLSound> m_Sounds;
		std::vector<SoundEvent> m_ActiveSounds;

		std::mutex m_Mutex;
	};
}
