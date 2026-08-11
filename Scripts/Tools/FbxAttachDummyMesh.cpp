#define FBXSDK_SHARED
#include <fbxsdk.h>

#include <iostream>
#include <string>

namespace
{
FbxNode* FindNodeBySuffix(FbxNode* Node, const std::string& Suffix)
{
    if (!Node)
    {
        return nullptr;
    }

    const std::string Name = Node->GetName();
    if (Name == Suffix ||
        (Name.size() > Suffix.size() &&
         Name.compare(Name.size() - Suffix.size(), Suffix.size(), Suffix) == 0))
    {
        return Node;
    }

    for (int Index = 0; Index < Node->GetChildCount(); ++Index)
    {
        if (FbxNode* Match = FindNodeBySuffix(Node->GetChild(Index), Suffix))
        {
            return Match;
        }
    }
    return nullptr;
}

void AddSkeletonNodesToBindPose(FbxNode* Node, FbxPose* BindPose)
{
    if (!Node || !BindPose)
    {
        return;
    }

    const FbxNodeAttribute* Attribute = Node->GetNodeAttribute();
    if (Attribute &&
        Attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
    {
        BindPose->Add(Node, Node->EvaluateGlobalTransform());
    }

    for (int Index = 0; Index < Node->GetChildCount(); ++Index)
    {
        AddSkeletonNodesToBindPose(Node->GetChild(Index), BindPose);
    }
}

int FindBinaryWriterFormat(FbxManager* Manager)
{
    FbxIOPluginRegistry* Registry = Manager->GetIOPluginRegistry();
    const int NativeFormat = Registry->GetNativeWriterFormat();
    for (int Index = 0; Index < Registry->GetWriterFormatCount(); ++Index)
    {
        if (!Registry->WriterIsFBX(Index))
        {
            continue;
        }
        const std::string Description = Registry->GetWriterFormatDescription(Index);
        if (Description.find("binary") != std::string::npos ||
            Description.find("Binary") != std::string::npos)
        {
            return Index;
        }
    }
    return NativeFormat;
}
}

int main(int ArgCount, char** Args)
{
    if (ArgCount != 3)
    {
        std::cerr << "Usage: FbxAttachDummyMesh <input.fbx> <output.fbx>\n";
        return 2;
    }

    FbxManager* Manager = FbxManager::Create();
    if (!Manager)
    {
        std::cerr << "Could not create FBX manager.\n";
        return 3;
    }

    FbxIOSettings* IOSettings = FbxIOSettings::Create(Manager, IOSROOT);
    Manager->SetIOSettings(IOSettings);
    FbxScene* Scene = FbxScene::Create(Manager, "MixamoAnimationWithDummyMesh");
    FbxImporter* Importer = FbxImporter::Create(Manager, "");

    if (!Importer->Initialize(Args[1], -1, Manager->GetIOSettings()) ||
        !Importer->Import(Scene))
    {
        std::cerr << "FBX import failed: " << Importer->GetStatus().GetErrorString()
                  << "\n";
        Importer->Destroy();
        Manager->Destroy();
        return 4;
    }
    Importer->Destroy();

    FbxNode* Hips = FindNodeBySuffix(Scene->GetRootNode(), "Hips");
    if (!Hips || !Hips->GetNodeAttribute() ||
        Hips->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eSkeleton)
    {
        std::cerr << "Could not find the Mixamo Hips skeleton node.\n";
        Manager->Destroy();
        return 5;
    }

    FbxMesh* Mesh = FbxMesh::Create(Scene, "NaturalNPC_DummyMesh");
    Mesh->InitControlPoints(3);
    FbxVector4* Points = Mesh->GetControlPoints();
    Points[0] = FbxVector4(-0.5, 0.0, 0.0);
    Points[1] = FbxVector4(0.5, 0.0, 0.0);
    Points[2] = FbxVector4(0.0, 0.5, 0.0);
    Mesh->BeginPolygon();
    Mesh->AddPolygon(0);
    Mesh->AddPolygon(1);
    Mesh->AddPolygon(2);
    Mesh->EndPolygon();

    FbxNode* MeshNode = FbxNode::Create(Scene, "NaturalNPC_DummyMesh");
    MeshNode->SetNodeAttribute(Mesh);
    Scene->GetRootNode()->AddChild(MeshNode);

    FbxSkin* Skin = FbxSkin::Create(Scene, "NaturalNPC_DummySkin");
    FbxCluster* Cluster = FbxCluster::Create(Scene, "NaturalNPC_HipsCluster");
    Cluster->SetLink(Hips);
    Cluster->SetLinkMode(FbxCluster::eTotalOne);
    for (int Index = 0; Index < 3; ++Index)
    {
        Cluster->AddControlPointIndex(Index, 1.0);
    }
    Cluster->SetTransformMatrix(MeshNode->EvaluateGlobalTransform());
    Cluster->SetTransformLinkMatrix(Hips->EvaluateGlobalTransform());
    Skin->AddCluster(Cluster);
    Mesh->AddDeformer(Skin);

    FbxPose* BindPose = FbxPose::Create(Scene, "NaturalNPC_BindPose");
    BindPose->SetIsBindPose(true);
    BindPose->Add(MeshNode, MeshNode->EvaluateGlobalTransform());
    AddSkeletonNodesToBindPose(Scene->GetRootNode(), BindPose);
    Scene->AddPose(BindPose);

    FbxExporter* Exporter = FbxExporter::Create(Manager, "");
    const int WriterFormat = FindBinaryWriterFormat(Manager);
    if (!Exporter->Initialize(Args[2], WriterFormat, Manager->GetIOSettings()) ||
        !Exporter->Export(Scene))
    {
        std::cerr << "FBX export failed: " << Exporter->GetStatus().GetErrorString()
                  << "\n";
        Exporter->Destroy();
        Manager->Destroy();
        return 6;
    }

    Exporter->Destroy();
    Manager->Destroy();
    std::cout << "Created FBX with dummy skinned mesh: " << Args[2] << "\n";
    return 0;
}
