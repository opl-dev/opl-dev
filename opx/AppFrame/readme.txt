// README.TXT
//
// Copyright (c) 1999-2000 Symbian Ltd.  All rights reserved.
//

AppFrame.OPX
============

Contents:

* Introduction
* Using the OPX
* OXH contents
* Procedure reference
* Copyright


INTRODUCTION
------------
This OPX allows OPL developers to use and manager the on-screen elements of an 
EPOC Crystal device family OPL application. Currently AppFrame supports the 
Command Button Array (CBA) controls (functionally equivalent to- and with the 
same API as toolbar buttons in future devices); the application title bar; and
the status indicator pane.

Typically, the OPX configures the CBA buttons so that they display the
appropriate labels for interacting with OPL applications, with the OPX
indicating a button has been pressed by calling back (running) a pre-agreed
OPL procedure.


USING THE OPX
-------------

1.  First translate and run the TAppFrame.OPL file to make sure everything
works correctly.

2.  To use the OPX in your program add the following line to the top of the
code, immediately after the APP...ENDA and before the first procedure

    INCLUDE "AppFrame.OXH"

3.  You can now use the application frame OPX procedures in your program.


CONTENTS OF OXH
---------------

REM AppFrame.OXH
REM
REM Copyright (c) 1999-2000 Symbian Ltd.  All rights reserved.
REM

const KUidAfOpx&=&10005235
const KVersionAfOpx%=$0001

const KAfStatusPaneTypeNarrow%=1
const KAfStatusPaneTypeWide%=2

DECLARE OPX AppFrame,KUidAfOpx&,KVersionAfOpx%
	AfOfferEvent%:(aEv1&,aEv3&,aEv4&,aEv5&,aEv6&,aEv7&) : 1
	AfSetCBAButton:(aButtonIndex%,aText$,aBitmapId%,aMaskId%,aCallback$) : 2
	AfSetCBAButtonDefault:(aButtonIndex%) : 3
	AfSetCBAButtonDimmed:(aButtonIndex%,aVisibility%) : 4
	AfCBAButtonDimmed%:(aButtonIndex%) : 5
	AfSetCBAButtonVisible:(aButtonIndex%,aVisibility%) : 6
	AfCBAButtonVisible%:(aButtonIndex%) : 7
	AfSetCBAVisible%:(aVisibility%) : 8
	AfCBAVisible%: : 9
	AfCBAMaxButtons%: : 10
	AfSetStatus%:(aType%) : 11
	AfSetStatusVisible%:(aVisibility%) : 12
	AfStatusVisible%:(BYREF aType%) : 13
	AfSetTitle:(aTitle$) : 14
	AfSetTitleVisible%:(aVisibility%) : 15
	AfTitleVisible%: : 16
	AfScreenInfo:(BYREF aXOrigin%, BYREF aYOrigin%, BYREF aWidth%, BYREF aHeight%) : 17
END DECLARE



PROCEDURE REFERENCE
-------------------


AfOfferEvent%:
--------------
Usage: AfOfferEvent%:(ev1&,ev3&,ev4&,ev5&,ev6&,ev7&)

Offers an event to the button group OPX, which returns KTrue% if the event
was used, and KFalse% otherwise. 
If the event was consumed by the OPX, you should take no further action with
it.


AfSetCBAButton:
---------------
Usage: AfSetCBAButton:(buttonIndex%, text$, bitmapId%, maskId%, callback$)

Initialises the button group button indicated by buttonIndex%, range 1 to 4,
with 1 at the top (or left) of the group and 4 at the bottom (right) of the
group.

You should call this each time you wish to change the button label, for
example, when displaying a different view of your OPL application.
If the button is pressed, the OPL procedure name passed in callback$ is run.


AfSetCBAButtonDefault:
----------------------
Usage: AfSetCBAButtonDefault:(buttonIndex%)

Sets a CBA button to be default. The default button is marked with underlined 
text, and is activated by the Enter key in addition to the CBA button.
Only one button can be marked default. Any existing default button is
no longer default.
Note: you may need to use this if writing your own dialog.


AfSetCBAButtonDimmed:
---------------------
Usage: AfSetCBAButtonDimmed:(buttonIndex%,dimmed%) 

Set a CBA button dimmed or undimmed, depending on the state of the flag
passed in dimmed% (KTrue% or KFalse%). The button position is passed in
buttonIndex%.


AfCBAButtonDimmed%:
-------------------
Usage: dimmed%=AfCBAButtonDimmed%:(buttonIndex%)

Tests a buttons dimmed state, return KTrue% if the button is dimmed, or
KFalse% if normal.


AfSetCBAButtonVisible:
----------------------
Usage: AfSetCBAButtonVisible:(buttonIndex%,visibility%)

Sets a CBA button visible or invisible, depending on the visibility% flag.
See AfSetCBAButtonDimmed: for more info.


AfCBAButtonVisible%:
--------------------
Usage: visible%=AfCBAButtonVisible%:(buttonIndex%)

Tests whether a button is visible or not, returning KTrue% if visible, or
KFalse% otherwise. See AfCBAButtonDimmed%: for more infomation.


AfSetCBAVisible%:
-----------------
Usage: redraw%=AfSetCBAVisible%:(visibility%)

Sets the entire CBA visible or invisible, depending on the flag passed in.
Returns KTrue% if the displayable screen size has changed because of this
action, KFalse% otherwise.


AfCBAVisible%:
--------------
Usage: visible%=AfCBAVisible%:

Tests whether the entire CBA is visible, returning KTrue% if visible, KFalse%
otherwise.


AfCBAMaxButtons%:
-----------------
Usage: count%=AfCBAMaxButtons%:

Returns the maximum number of buttons available for the current device.


AfSetStatus%:
-------------
Usage: redraw%=AfSetStatus%:(type%)

Sets the status indicator pane to the type passed in the type% flag, which
must be one of:

	const KAfStatusPaneTypeNarrow%=1
	const KAfStatusPaneTypeWide%=2

Returns KTrue% if the displayable screen size changed as a result of this
operation, KFalse% otherwise.


AfSetStatusVisible%:
--------------------
Usage: redraw%=AfSetStatusVisible%:(visibility%)

Sets the status indicator pane visible or invisible, depending on the
visibility% flag passed in.
Returns KTrue% if the displayable screen size has changed as a result,
KFalse% otherwise.


AfStatusVisible%:
-----------------
Usage: visible%=AfStatusVisible%:(type%)

Tests the status pane indicator, returning KTrue% if visible, KFalse%
otherwise. In addition, the type% flag is set to one of:

	const KAfStatusPaneTypeNarrow%=1
	const KAfStatusPaneTypeWide%=2

to indicate the type of status pane in use.


AfSetTitle:
-----------
Usage: AfSetTitle:(title$)

Sets the application title bar to the Unicode text string passed in title$.
Note that the string is ignored if 40 or more characters in length.


AfSetTitleVisible%:
-------------------
Usage: redraw%=AfSetTitleVisible%:(visibility%)

Sets the application title bar visible or not, depending on the visibility%
flag passed in.
Returns KTrue% if the displayable screen size has changed as a result of this
operation, KFalse% otherwise.


AfTitleVisible%:
----------------
Usage: visible%=AfTitleVisible%:

Tests whether the application title bar is visible, returning KTrue% if
visible, or KFalse% otherwise.


AfScreenInfo:
-------------
Usage: AfScreenInfo:(x%,y%,width%,height%)

Sets the variables passed in to the following values:
 x%       x position of visible window origin
 y%       y position of visible window origin
 width%   width in pixels of visible window
 height%  height in pixels of visible window

These values can be used with gSETWIN to change the size and position of the
current window in order for it to adjust to the on-screen items.


AfCreateCBAcontainer&:
----------------------
Usage: id&=AfCreateCBAContainer&:

Creates a new CBA container, and makes it the default.
Returns a handle to the container.


AfUseCBAContainer:
------------------
Usage: AfUseCBAContainer:(id&)

Makes the CBA container whose handle is id& current. All AppFrame CBA functions will now
operate on this CBA container.


AfDeleteCBAContainer:
---------------------
Usage: AfDeleteCBAContainer:(id&)

Deletes the CBA container whose handle is id&. If this container was the default, the 
last valid CBA container created becomes the default.



COPYRIGHT
---------

AppFrame.opx is copyright (c) 1999-2000 Symbian Ltd.  All rights reserved.  It
forms part of the Symbian OPL SDK and is subject to the License contained
therein.

