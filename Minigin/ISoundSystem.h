#pragma once
#include <cstdint>
#include <string>

namespace dae
{
	class ISoundSystem
	{
	public:
		ISoundSystem( ) = default;

		virtual void PushSound( uint16_t soundId, float volume ) = 0;
		virtual void ProcessSounds( ) = 0;
		virtual uint16_t GetSound( const std::string& fileName ) = 0;
		virtual void Init( ) = 0;
	
		// Rule of 5
		virtual ~ISoundSystem( ) = default;
		ISoundSystem( const ISoundSystem& other ) = delete;
		ISoundSystem( ISoundSystem&& other ) noexcept = delete;
		ISoundSystem& operator=( const ISoundSystem& other ) = delete;
		ISoundSystem& operator=( ISoundSystem&& other ) noexcept = delete;
	};
}
