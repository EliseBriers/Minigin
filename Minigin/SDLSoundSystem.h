#pragma once
#include "ISoundSystem.h"
#include "ManagerMap.h"
#include <mutex>
#include "UUID.h"

struct Mix_Chunk;

namespace dae
{
	class ResourceManager;

	class SDLSoundSystem final : public ISoundSystem
	{
	public:
		struct SDLSound
		{
			SDLSound( Mix_Chunk* p );

			Mix_Chunk* pChunk;
			const UUID<SDLSound> Id;
		};

	private:
		// struct SDLSound
		// {
		// 	SDLSound( );
		// 	SDLSound( uint16_t id, const std::string& fileName, Mix_Chunk* pChunk );
		// 
		// 	uint16_t Id;
		// 	std::string Name;
		// 	Mix_Chunk* pSound;
		// };

		struct SoundEvent
		{
			SoundEvent( );
			SoundEvent( size_t id, float volume );;

			size_t Id;
			float Volume;
		};

	public:
		SDLSoundSystem( ResourceManager& resourceManager );

		void PushSound( size_t soundId, float volume ) override;
		void ProcessSounds( ) override;
		size_t GetSound( const std::string& fileName ) override;
		void Init( ) override;

		// Rule of 5
		~SDLSoundSystem( ) = default;
		SDLSoundSystem( const SDLSoundSystem& other ) = delete;
		SDLSoundSystem( SDLSoundSystem&& other ) noexcept = delete;
		SDLSoundSystem& operator=( const SDLSoundSystem& other ) = delete;
		SDLSoundSystem& operator=( SDLSoundSystem&& other ) noexcept = delete;

	private:
		Mix_Chunk* GetChunk( size_t id );
		const SDLSound* FindSoundWithId( size_t id );
		std::vector<SoundEvent> m_ActiveSounds;
		std::vector<std::reference_wrapper<const SDLSound>> m_Sounds;

		ResourceManager& m_ResourceManager;

		std::mutex m_Mutex;
	};
}
