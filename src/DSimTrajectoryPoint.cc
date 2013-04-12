#include "DSimRootGeometryManager.hh"
#include "DSimTrajectoryPoint.hh"

#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4StepStatus.hh>

#include <G4AttDefStore.hh>
#include <G4AttDef.hh>
#include <G4AttValue.hh>
#include <G4UnitsTable.hh>

#include <TGeoManager.h>

#include <DSimLog.hh>

G4Allocator<DSimTrajectoryPoint> aDSimTrajPointAllocator;

DSimTrajectoryPoint::DSimTrajectoryPoint()
    : fTime(0.), fMomentum(0.,0.,0.),
      fStepStatus(fUndefined), 
      fPhysVolName("OutofWorld"), fPrevPosition(0,0,0) { }

DSimTrajectoryPoint::DSimTrajectoryPoint(const G4Step* aStep)
    : G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition()) {
    fTime = aStep->GetPostStepPoint()->GetGlobalTime();
    fMomentum = aStep->GetPostStepPoint()->GetMomentum();
    fStepStatus = aStep->GetPostStepPoint()->GetStepStatus();
    if (aStep->GetPostStepPoint()->GetPhysicalVolume()) {
        fPhysVolName 
            = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    }
    else {
        fPhysVolName == "OutOfWorld";
    }
    fPrevPosition = aStep->GetPreStepPoint()->GetPosition();
}

DSimTrajectoryPoint::DSimTrajectoryPoint(const G4Track* aTrack)
    : G4TrajectoryPoint(aTrack->GetPosition()) {
    fTime = aTrack->GetGlobalTime();
    fMomentum = aTrack->GetMomentum();
    fStepStatus = fUndefined;
    if (aTrack->GetVolume()) {
        fPhysVolName = aTrack->GetVolume()->GetName();
    }
    else {
        fPhysVolName == "OutOfWorld";
    }
    fPrevPosition = aTrack->GetPosition();
}

DSimTrajectoryPoint::DSimTrajectoryPoint(const DSimTrajectoryPoint &right)
    : G4TrajectoryPoint(right) {
    fTime = right.fTime;
    fMomentum = right.fMomentum;
    fStepStatus = right.fStepStatus;
    fPhysVolName = right.fPhysVolName;
    fPrevPosition = right.fPrevPosition;
}

DSimTrajectoryPoint::~DSimTrajectoryPoint() { }

const std::map<G4String,G4AttDef>* DSimTrajectoryPoint::GetAttDefs() const {
    G4bool isNew;

    std::map<G4String,G4AttDef>* store
        = G4AttDefStore::GetInstance("DSimTrajectoryPoint",isNew);

    if (isNew) {

        G4String Time("Time");
        (*store)[Time] =
            G4AttDef(Time, "Time", "Physics","G4BestUnit","G4double");

        G4String Momentum("Momentum");
        (*store)[Momentum] =
            G4AttDef(Momentum, "Momentum", "Physics",
                     "G4BestUnit","G4ThreeVector");

        G4String StepStatus("StepStatus");
        (*store)[StepStatus] =
            G4AttDef(StepStatus, "StepStatus", "Physics", "", "G4StepStatus");

        G4String VolumeName("VolumeName");
        (*store)[VolumeName] =
            G4AttDef(VolumeName, "VolumeName", "Physics", "", "G4String");

        G4String VolumeNode("VolumeNode");
        (*store)[VolumeNode] =
            G4AttDef(VolumeNode, "VolumeNode", "Physics", "", "G4int");

    }
    return store;
}

std::vector<G4AttValue>* DSimTrajectoryPoint::CreateAttValues() const {
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

    values->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

    values->push_back(G4AttValue("Momentum",
                                 G4BestUnit(fMomentum,"Momentum"),""));
    values->push_back(G4AttValue("StepStatus",fStepStatus,""));

    values->push_back(G4AttValue("PhysVolName",fPhysVolName,""));

#ifdef G4ATTDEBUG
    DSimInfo(G4AttCheck(values,GetAttDefs()));
#endif

    return values;
}

int DSimTrajectoryPoint::GetVolumeNode() const {
    gGeoManager->PushPath();
    int node
        = DSimRootGeometryManager::Get()->GetNodeId(0.5*(GetPosition()
                                                          +fPrevPosition));
    gGeoManager->PopPath();
    return node;
}