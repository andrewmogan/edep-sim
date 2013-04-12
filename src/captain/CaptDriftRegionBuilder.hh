#ifndef CaptDriftRegionBuilder_hh_Seen
#define CaptDriftRegionBuilder_hh_Seen

#include "DSimBuilder.hh"

class G4LogicalVolume;

/// Construct the drift region of the TPC.
class CaptDriftRegionBuilder : public DSimBuilder {
public:
    CaptDriftRegionBuilder(G4String name, DSimBuilder* parent)
        : DSimBuilder(name,parent) {Init();};
    virtual ~CaptDriftRegionBuilder();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the total height of the drift region.
    double GetHeight();

    /// Set the radius of the largest circle that can be contained in the
    /// drift region.
    /// @{
    void SetApothem(double v) {fApothem = v;}
    double GetApothem() {return fApothem;}
    /// @}

    /// Set the length of the drift region from the cathode to the first wire
    /// plane.
    /// @{
    void SetDriftLength(double v) {fDriftLength = v;}
    double GetDriftLength() {return fDriftLength;}
    /// @}
    
    /// Set the spacing between the wire planes.
    void SetWirePlaneSpacing(double v) {fWirePlaneSpacing = v;}
    double GetWirePlaneSpacing() {return fWirePlaneSpacing;}
    /// @}

    
private:
    void Init(void);

    /// The radius of the circle that can fit inside.
    double fApothem;

    /// The distance from the cathode to the first wire plane.
    double fDriftLength;

    /// The distance between the 3 wire planes.
    double fWirePlaneSpacing;

};
#endif