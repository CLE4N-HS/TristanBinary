#include <iostream>

enum Option
{
	FULLSCREEN = 1 << 0,
	GENERAL_MUTED = 1 << 1,
	MUSIC_MUTED = 1 << 2,
	SFX_MUTED = 1 << 3,
	V_SYNC = 1 << 4,
};

int main()
{
	uint8_t o = MUSIC_MUTED | V_SYNC;

	uint8_t mask = MUSIC_MUTED;

	bool isFullScreen = ((o & FULLSCREEN) != 0);

	o |= FULLSCREEN;
	bool isNowFullScreen = ((o & FULLSCREEN) != 0);

	o ^= SFX_MUTED; // toggle a bit
	bool HasSfxMuted = ((o & SFX_MUTED) != 0);
	o ^= SFX_MUTED;
	bool HasSfxMuted2 = ((o & SFX_MUTED) != 0);
	o ^= SFX_MUTED;
	bool HasSfxMuted3 = ((o & SFX_MUTED) != 0);

	bool hasVSync = o & V_SYNC;
	o &= ~V_SYNC; // force a bit to false
	bool hasVSync2 = o & V_SYNC;
	o &= ~V_SYNC;
	bool hasVSync3 = o & V_SYNC;

	bool hasGeneralMuted = o & GENERAL_MUTED;
	o |= GENERAL_MUTED; // force a bit to true
	bool hasGeneralMuted1 = o & GENERAL_MUTED;
	o |= GENERAL_MUTED;
	bool hasGeneralMuted2 = o & GENERAL_MUTED;

	return EXIT_SUCCESS;
}