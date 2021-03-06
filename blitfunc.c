#include "sysconfig.h"
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "custom.h"
#include "memory.h"
#include "blitter.h"
#include "blitfunc.h"

void blitdofast_0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (0) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd; ++ptd;}
	}
	if (ptd) ptd += b->bltdmod;
}
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (0) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (ptd) ptd -= b->bltdmod;
}
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc; ++ptc;}
		if (pta) { bltadat = chipmem_wget (pta); ++pta; ++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((~srca & srcc)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;  ++ptd;}
	}
	if (pta) pta += b->bltamod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((~srca & srcc)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_2a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc & ~(srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_2a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc & ~(srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_30 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca & ~srcb)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_30 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca & ~srcb)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_3a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcb ^ (srca | (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_3a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcb ^ (srca | (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_3c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ srcb)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_3c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ srcb)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_4a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb | srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_4a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb | srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_6a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_6a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_8a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (~srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_8a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (~srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_8c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcb & (~srca | srcc))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_8c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcb & (~srca | srcc))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_9a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & ~srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_9a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & ~srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_a8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_a8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_aa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (srcc) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_aa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (srcc) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_b1 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (~(srca ^ (srcc | (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_b1 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (~(srca ^ (srcc | (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_ca (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_ca (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_cc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (srcb) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_cc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (srcb) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_d8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ (srcc & (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_d8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ (srcc & (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_e2 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srcb & (srca ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_e2 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srcb & (srca ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_ea (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc | (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_ea (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srcc | (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_f0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (srca) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptd) ptd += b->bltdmod;
}
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_f0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = (srca) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptd) ptd -= b->bltdmod;
}
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_fa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = chipmem_wget (ptc); ++ptc;++ptc; }
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcc)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_fa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcc)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_fc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ++ptb;++ptb;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); ++pta;++pta; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcb)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ++ptd;++ptd; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_fc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = 0; j < b->vblitsize; j++) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (ptb) {
			uae_u32 bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dstp) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcb)) & 0xFFFF;
		totald |= dstd;
		if (ptd) { dstp = ptd; ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
		if (dstp) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
