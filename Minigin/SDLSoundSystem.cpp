#include "MiniginPCH.h"
#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include "ResourceManager.h"
#include "Logger.h"

// dae::SDLSoundSystem::SDLSound::SDLSound( )
// 	: SDLSound{ 0, "", nullptr }
// {
// }
// 
// dae::SDLSoundSystem::SDLSound::SDLSound( uint16_t id, const std::string& fileName, Mix_Chunk* pChunk )
// 	: Id{ id }
// 	, Name{ fileName }
// 	, pSound{ pChunk }
// {
// }

dae::SDLSoundSystem::SDLSound::SDLSound( Mix_Chunk* p )
	: pChunk{ p }
{
}

dae::SDLSoundSystem::SoundEvent::SoundEvent( )
	: SoundEvent{ 0, 0.f }
{
}

dae::SDLSoundSystem::SoundEvent::SoundEvent( size_t id, float volume )
	: Id{ id }
	, Volume{ volume }
{
}

dae::SDLSoundSystem::SDLSoundSystem( ResourceManager& resourceManager )
	: m_ResourceManager{ resourceManager }
{
}

void dae::SDLSoundSystem::PushSound( size_t soundId, float volume )
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

	SoundEvent& lastSound{ m_ActiveSounds[m_ActiveSounds.size( ) - 1] };
	Mix_Chunk* const pChunk{ GetChunk( lastSound.Id ) };
	if( !pChunk )
	{
		Logger::LogWarning( "dae::SDLSoundSystem::ProcessSounds > pChunk is nullptr" );
		return;
	}
	Mix_VolumeChunk( pChunk, static_cast<int>(lastSound.Volume) );
	Mix_PlayChannel( -1, pChunk, 0 );
	m_ActiveSounds.pop_back( );
}

size_t dae::SDLSoundSystem::GetSound( const std::string& fileName )
{
	std::lock_guard<std::mutex> lock{ m_Mutex };

	const SDLSound& sound{ m_ResourceManager.GetSDLSound( fileName ) };
	m_Sounds.emplace_back( sound );
	return sound.Id.GetValue( );
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

Mix_Chunk* dae::SDLSoundSystem::GetChunk( size_t id )
{
	return FindSoundWithId( id )->pChunk;
}

const dae::SDLSoundSystem::SDLSound* dae::SDLSoundSystem::FindSoundWithId( size_t id )
{
	for( std::reference_wrapper<const SDLSound> ref : m_Sounds )
	{
		if( ref.get( ).Id.GetValue( ) == id )
			return &ref.get( );
	}
	return nullptr;
}
