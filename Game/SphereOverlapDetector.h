#pragma once
#include <IComponent.h>
#include "MathHelpers.h"
#include "Enums.h"

class SphereOverlapDetector final : public dae::IComponent
{
	struct OverlapData
	{
		dae::GameObject* pOther;
		bool HasNoOverlap;
	};

public:

	using callback_t = std::function<void( dae::GameObject*, TriggerAction )>;

	SphereOverlapDetector( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );
	void Update( const dae::UpdateInfo& ) override;
	void Draw( dae::Renderer& ) override;
	void Init( const dae::InitInfo& ) override;

	void SetCallback( const callback_t& callback );

	void DetectOverlap( SphereOverlapDetector& other );
	
	// Rule of 5
	~SphereOverlapDetector( ) override = default;
	SphereOverlapDetector( const SphereOverlapDetector& other ) = delete;
	SphereOverlapDetector( SphereOverlapDetector&& other ) noexcept = delete;
	SphereOverlapDetector& operator=( const SphereOverlapDetector& other ) = delete;
	SphereOverlapDetector& operator=( SphereOverlapDetector&& other ) noexcept = delete;
private:
	callback_t m_Callback;
	MathHelpers::Circle m_Circle;
	const float m_Radius;
	dae::TransformComponent* m_pTransform;

	std::vector<OverlapData> m_Overlaps;

	void RegisterOverlap( dae::GameObject* pOther );
	void UpdateCircle( );
	static void DefaultCallback( dae::GameObject*, TriggerAction );
	static bool HasNoOverlap( const OverlapData& overlap );
};
