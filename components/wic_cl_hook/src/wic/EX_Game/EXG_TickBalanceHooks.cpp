#include <Hooks/Hooks.h>

double hk_call_00705A12(double Value);

//
// Fix for the rounding error in team tick rates ("Dom Bar Bug") when playing domination. Under certain conditions, such as when NonLinearDomination
// is enabled, one team would have a small advantage throughout the match. This varies depending on the map parameters and captured command points.
//
// Simplified equation:
// C1                = # of captured command points by Team 1
// C2                = # of captured command points by Team 2
// NonLinearDomScale = Arbitrary point on a spline (between 0 and 1)
// TickRate          = abs( ceil( (C1 - C2) * NonLinearDomScale ) )
//
// Example 1 (Team 1 gaining ticks):
// C1                = 4
// C2                = 1
// NonLinearDomScale = 0.95
// TickRate          = abs(ceil((4 - 1) * 0.95)) = abs(ceil(2.85)) = 3 ticks per second 
//
// Example 2 (Team 2 gaining ticks) (Command point counts swapped):
// C1                = 1
// C2                = 4
// NonLinearDomScale = 0.95
// TickRate          = abs(ceil((1 - 4) * 0.95)) = abs(ceil(-2.85)) = 2 ticks per second
//
// Team 2 is at a disadvantage because ceil() rounds **negative** numbers towards zero. The patch applies floor() to negative numbers (Example 2) and
// ceil() to positive numbers (Example 1) so both teams would have an equal tick rate. This bug has been present since the game was released in 2007.
//
// Special thanks to Cat, Gut Plaer, and the community for investigation. Confirmed as a bug by former WIC developers.
//
DECLARE_HOOK_INITIALIZER_MP(EXG_TickBalance,
{
	Hooks::WriteCall(0x00705A12, &hk_call_00705A12);
});

DECLARE_HOOK_INITIALIZER_DS(EXG_TickBalance,
{
	Hooks::WriteCall(0x00501B99, &hk_call_00705A12);
});

double hk_call_00705A12(double Value)
{
	if (Value < 0)
		return floor(Value);

	return ceil(Value);
}