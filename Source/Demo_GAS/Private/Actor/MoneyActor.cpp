#include "Actor/MoneyActor.h"
#include <Kismet/DataTableFunctionLibrary.h>

AMoneyActor::AMoneyActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

}

void AMoneyActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//if (MoneysDataTable)
	if (!MoneysDataTable.RowName.IsNone())
	{
		/* 在蓝图里需要用GetDataTableRow节点获取行数据，对应的源码函数是：GetDataTableRowFromName，
		* 但是在C++里直接用 FindRow 就可以获取行数据了！非常简单！*/
		FMoney* row = MoneysDataTable.DataTable->FindRow<FMoney>(MoneysDataTable.RowName, FString("Blue"));
		Mesh->SetStaticMesh(row->Mesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Moneys的数据表格没配置！"));
	}
	
}

void AMoneyActor::BeginPlay()
{
	Super::BeginPlay();
	
}


