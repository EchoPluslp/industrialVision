
#include "transmitsignals.h"

TransmitSignals::TransmitSignals()
{

}

TransmitSignals::~TransmitSignals()
{

}
TransmitSignals& TransmitSignals::GetInstance()
{
    static TransmitSignals RobotControl;
    return RobotControl;
}
