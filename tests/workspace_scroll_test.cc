#include <cassert>
#include <limits>
#include "chrome/browser/ui/views/helium/workspace_scroll_tracker.h"

int main() {
  // Smooth trackpad samples accumulate into one switch, and the same gesture
  // cannot immediately switch the destination profile a second time.
  WorkspaceScrollTracker trackpad;
  assert(trackpad.Consume(-12, 1000) == 0);
  assert(trackpad.Consume(-12, 1020) == 0);
  assert(trackpad.Consume(-12, 1040) == 0);
  assert(trackpad.Consume(-12, 1060) == 1);
  assert(trackpad.Consume(-120, 1100) == 0);
  assert(trackpad.Consume(-120, 1200) == 0);
  assert(trackpad.Consume(-120, 1350) == 0);
  assert(trackpad.Consume(-120, 1500) == 0);
  assert(trackpad.Consume(-120, 1650) == 0);
  assert(trackpad.Consume(-120, 1800) == 0);
  assert(trackpad.Consume(60, 2100) == -1);

  // Separate mouse-wheel gestures can cycle deliberately, with a cooldown
  // guarding against repeated samples delivered to different profile windows.
  WorkspaceScrollTracker wheel;
  assert(wheel.Consume(-120, 10) == 1);
  assert(wheel.Consume(-120, 100) == 0);
  assert(wheel.Consume(-120, 600) == 1);
  assert(wheel.Consume(120, 1200) == -1);

  // Reversing direction cancels the old accumulation; idle motion is not
  // carried into a later gesture, and non-finite device samples are ignored.
  WorkspaceScrollTracker reversal;
  assert(reversal.Consume(24, 10) == 0);
  assert(reversal.Consume(-24, 20) == 0);
  assert(reversal.Consume(-24, 30) == 1);
  assert(reversal.Consume(24, 1000) == 0);
  assert(reversal.Consume(24, 1400) == 0);
  assert(reversal.Consume(std::numeric_limits<float>::quiet_NaN(), 1410) == 0);
  assert(reversal.Consume(24, 1420) == -1);
}
