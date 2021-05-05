#include "MiniginPCH.h"
#include "SDLSoundSystem.h"
#include <SDL_mixer.h>

dae::SDLSoundSystem::SDLSound::SDLSound( )
	: SDLSound{ 0, "", nullptr }
{
}

dae::SDLSoundSystem::SDLSound::SDLSound( uint16_t id, const std::string& fileName, Mix_Chunk* pChunk )
	: Id{ id }
	, Name{ fileName }
	, pSound{ pChunk }
{
}

dae::SDLSoundSystem::SoundEvent::SoundEvent( )
	: SoundEvent{ 0, 0.f }
{
}

dae::SDLSoundSystem::SoundEvent::SoundEvent( uint16_t id, float volume )
	: Id{ id }
	, Volume{ volume }
{
}

dae::SDLSoundSystem::SDLSoundSystem( ) = default;

void dae::SDLSoundSystem::PushSound( uint16_t soundId, float volume )
{
	std::lock_guard<std::mutex> lock{ m_Mutex };

	const auto it{
		std::find_if( m_ActiveSounds.begin( ), m_ActiveSounds.end( ), [soundId]( const SoundEvent& se )
		{
			return se.Id == soundId;
		} )
	};
	if( it == m_ActiveSounds.end( ) )
		m_ActiveSounds.emplace_back( soundId, volume );
	else
		it->Volume = std::max( volume, it->Volume );
}

void dae::SDLSoundSystem::ProcessSounds( )
{
	std::lock_guard<std::mutex> lock{ m_Mutex };
	if( m_ActiveSounds.empty( ) )
		return;
	// If all channels are already playing, return
	if( Mix_Playing( -1 ) == 4 )
		return;

	SoundEvent & lastSound{ m_ActiveSounds[m_ActiveSounds.size( ) - 1] };
	Mix_Chunk* const pChunk{ GetChunk( lastSound.Id ) };
	Mix_VolumeChunk( pChunk, static_cast<int>(lastSound.Volume) );
	Mix_PlayChannel( -1, pChunk, 0 );
	m_ActiveSounds.pop_back( );
}

uint16_t dae::SDLSoundSystem::GetSound( const std::string& fileName )
{
	std::lock_guard<std::mutex> lock{ m_Mutex };
	const auto it{
		std::find_if( m_Sounds.begin( ), m_Sounds.end( ), [&fileName]( const SDLSound& s )
		{
			return s.Name == fileName;
		} )
	};

	if( it != m_Sounds.end( ) )
		return it->Id;

	const char* pFileName{ fileName.c_str( ) };
	Mix_Chunk* pSound{ Mix_LoadWAV( pFileName ) };


	uint16_t newId{ static_cast<uint16_t>(m_Sounds.size( )) };
	m_Sounds.emplace_back( newId, fileName, pSound );
	return newId;
}

void dae::SDLSoundSystem::Init( )
{
	const int audio_rate{ 44100 };
	const Uint16 audio_format{ MIX_DEFAULT_FORMAT };
	const int audio_channels{ 1 };
	const int audio_buffers{ 4096 };

	if( Mix_OpenAudio( audio_rate, audio_format, audio_channels, audio_buffers ) < 0 )
	{
		printf( "Unable to open audio!\n" );
		exit( 1 );
	}

	if( Mix_Init( MIX_INIT_MOD ) != MIX_INIT_MOD )
		std::cout << "error";
}

Mix_Chunk* dae::SDLSoundSystem::GetChunk( uint16_t id )
{
	return m_Sounds[id].pSound;
}
