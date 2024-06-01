#pragma once

UENUM()
enum class EItemType : uint8
{
	EIT_PhysicsObject,
	EIT_Tool
};

UENUM()
enum class EInputWay :uint8
{
	EIW_Click,
	EIW_Holding,
	EIW_Mashing
};

UENUM()
enum class EInteractableResponse :uint8
{
	EIR_Persistent,
	EIR_OnlyOnce,
	EIR_Temporary
};

UENUM()
enum class EItemsType : uint8
{
	EIT_Object,
	EIT_Weapon,
	EIT_Shield,
	EIT_Food,
	
};
