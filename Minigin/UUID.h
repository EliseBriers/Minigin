#pragma once

namespace dae
{
	template <typename T>
	class UUID final
	{
	public:
		UUID( );
		bool operator==( const UUID<T>& other ) const;
		bool operator!=( const UUID<T>& other ) const;
		size_t GetValue( ) const;

		UUID( const UUID& other ) = delete;
		UUID( UUID&& other ) noexcept = delete;
		UUID& operator=( const UUID& other ) = delete;
		UUID& operator=( UUID&& other ) noexcept = delete;
	private:
		const size_t m_Id;
		static size_t m_LastID;
	};

	template <typename T>
	size_t UUID<T>::m_LastID{ };

	template <typename T>
	UUID<T>::UUID( )
		: m_Id{ m_LastID++ }
	{
	}

	template <typename T>
	bool UUID<T>::operator==( const UUID<T>& other ) const
	{
		return other.m_Id == m_Id;
	}

	template <typename T>
	bool UUID<T>::operator!=( const UUID<T>& other ) const
	{
		return !( *this == other );
	}

	template <typename T>
	size_t UUID<T>::GetValue( ) const
	{
		return m_Id;
	}
}
