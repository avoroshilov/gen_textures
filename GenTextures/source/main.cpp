#include <stdio.h>
#include <conio.h>

#include <Windows.h>

#include "windows/timer.h"
#include "textures/GenTextures.h"

class ZoneAutoProfiler
{
public:

	typedef void (*TimingCallback)(const char *, double);

private:

	windows::Timer m_timer;
	const char * m_name;
	TimingCallback m_callback;

public:

	// Not exactly safe to pass pointers like that
	//	but for this demonstaration will do
	ZoneAutoProfiler(const char * name, TimingCallback callback):
		m_name(name),
		m_callback(callback)
	{
		m_timer.start();
	}
	~ZoneAutoProfiler()
	{
		double time = m_timer.time();
		if (m_callback)
			m_callback(m_name, time);
	}
};

int main(int argc, char ** argv)
{
	using namespace textures;

	TexPregen mainPregen;
	TexGen 
		testTex1,
		testTex2,
		bricksTex,
		concreteTex0(512, 512, 3),
		concreteTex1(512, 512, 3),
		concreteTex2(512, 512, 4),
		grassTex(512, 512, 4),
		leatherTex(512, 512, 4),
		linkTex0(256, 256, 3),
		detailMapTex,
		bloodTex(128, 128),
		lightTex(128, 128),
		fireTex(128, 128),
		dustTex(64, 64),
		bloodDecal0,
		bloodSplatter0Tex, bloodSplatter1Tex, bloodSplatter2Tex,
		bulletHoleTex(32, 32),
		polishedMetalTex(512, 512),
		rustyTex(1024, 1024, 10),
		emptyTex(1, 1, 1),
		fractalColormapTex(256, 1, 1),
		lightShaftsPatternTex(512, 512, 1),
		lensFlareDirtTex(512, 512, 2);

	// Initialize lookup arrays
	mainPregen.InitEnv();		// Basic radial gradient
	mainPregen.InitBias();		// Power/gamma-like
	mainPregen.InitRawMemChunk(1024 * 1024 * 20);	// 20 MB

	TexGen::SetRawChunk(mainPregen.RawChunkOfMem, mainPregen.RawChunkOfMemSize);

	ZoneAutoProfiler::TimingCallback profileCallback = [](const char * name, double time)
	{
		printf("Time \"%s\": %f ms\n", name, time);
	};

#define TEX_FOLDER "generated\\"
	CreateDirectoryA(TEX_FOLDER, NULL);

#define MAKE_STRING(object) \
			#object
#define MAKE_TEX_NAME(texture, index) \
			TEX_FOLDER #texture "_" #index ".tga"

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(fireTex), profileCallback);

		//TexGen fireTex(128, 128);
		fireTex.FractalP(0x00, 0x48);
		fireTex.MkChannel(0x00);
		fireTex.Contrast(0, 128, 24);
		//fireTex.Bias(0, 200, Pregen01.BiasArray);
		fireTex.Bright(0, 40);
		fireTex.Env(1, 0, 30, 0);
		fireTex.Mul(0, 1);
		fireTex.MakeAlpha(0x00);
		fireTex.MakeAlpha(0x11);
		fireTex.SaveToTGA(0, MAKE_TEX_NAME(fireTex, 0));
		fireTex.SaveToTGA(1, MAKE_TEX_NAME(fireTex, 1));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(testTex1), profileCallback);

		//TexGen testTex1(256, 256);
#if 1
		testTex1.FractalP(0x00, 0x38);
		testTex1.MkChannel(0x00);
		testTex1.FractalP(0x10, 0x58);
		testTex1.Glass(0, 0x10, 240);
		testTex1.Balance(0x02, 64, 128);
		testTex1.Balance(0x01, 96, 200);
		testTex1.Balance(0x00, 128, 255);
		//testTex1.Rect(0x00, 10, 128, 10, 255, 255, 0);
		//testTex1.Rect(0x01, 30, 191, 10, 127, 255, 0);
		testTex1.Add(0x20);
#else
		testTex1.FillColor(0, 255, 255, 255);
		testTex1.FractalP(0x20, 0x38);
		testTex1.Balance(0x20, 0, 64);
		testTex1.MkChannel(0x20);
#endif
		testTex1.Contrast(2, 128, 12);
		//testTex1.Rect(0x21, 128, 128, 128, 128, 255, 0);
		//testTex1.Blur(2, 2);
		testTex1.MakeAlpha(0x22);
		testTex1.MakeAlpha(0);
		testTex1.NMapSobel(1, 0x23);
		testTex1.Xchg(0x23, 0x13);
		testTex1.SaveToTGA(0, MAKE_TEX_NAME(testTex1, 0));
		testTex1.SaveToTGA(1, MAKE_TEX_NAME(testTex1, 1));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(detailMapTex), profileCallback);

		detailMapTex.Cells(0, 0x20, 128, 255, 0, mainPregen.EnvArray);
		detailMapTex.MakeAlpha(0);
		detailMapTex.Add(0x20);
		detailMapTex.MakeAlpha(0x22);
		detailMapTex.NMapSobel(1, 0x23);
		detailMapTex.Contrast(1, 128, 96);
		detailMapTex.Xchg(0x23, 0x13);

		detailMapTex.Balance(0x00, 200, 255);
		detailMapTex.MkChannel(0x00);
		//detailMapTex.Contrast(0, 128, 8);
		//detailMapTex.Bright(0, 64);

		//detailMapTex.FractalP(0x00, 0x38);
		//detailMapTex.Balance(0x00, 200, 255);
		//detailMapTex.MkChannel(0x00);
		//detailMapTex.MakeAlpha(0);

		detailMapTex.MakeAlpha(0x00);
		detailMapTex.MakeAlpha(0x11);
		detailMapTex.SaveToTGA(0, MAKE_TEX_NAME(detailMapTex, 0));
		detailMapTex.SaveToTGA(1, MAKE_TEX_NAME(detailMapTex, 1));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(bricksTex), profileCallback);

		bricksTex.FractalP(0x00, 0x38);
		bricksTex.Balance(0x00, 128, 190);
		bricksTex.MkChannel(0x00);
		bricksTex.Bricks(0x20, 4, 8, 4, 8, 10, 32, (8 << 16) + 8);
		bricksTex.MkChannel(0x20);
		bricksTex.Blur(2, 3);
		bricksTex.Add(0x12);
		bricksTex.MkChannel(0x10);
		bricksTex.Emboss(1, 16);
		bricksTex.Shade(0, 0x10);
		bricksTex.Contrast(2, 128, 2);		// That will make normal map less sharp
		bricksTex.MakeAlpha(0x22);			// Alpha from Layer2 to Layer2
		bricksTex.NMapSobel(1, 0x23);		// Normal map from Layer2 to Layer1
		bricksTex.Xchg(0x23, 0x13);			// Copy alpha from Layer2 to Layer1
		bricksTex.MakeAlpha(0x00);
		bricksTex.SaveToTGA(0, MAKE_TEX_NAME(bricksTex, 0));
		bricksTex.SaveToTGA(1, MAKE_TEX_NAME(bricksTex, 1));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(grassTex), profileCallback);

		// Grass generation test
		// By changing initial fractal plasm parameters - we're getting different
		// "leaf" structure. The less fractal plasm step, the more "uniform" leafs
		// are, if to increase step, leaf color varies with greater octave modulation
		//////////////////////////////////////////////////////////////////////////

#define TG_GRASS_SIMPLE 1

		grassTex.FractalP(0x00, 0x35, 255);
		//	grassTex.FractalP(0x00, 0x76, 255);
		grassTex.Diff(0x00, 0);
#if (TG_GRASS_SIMPLE == 0)
		grassTex.NormalizeCh(0x00, 0, 255);
		grassTex.BlurGauss(0, 2);			// halfRad = 1 gives more "groundy" look
		grassTex.Diff(0x00, 0);
		grassTex.NormalizeCh(0x00, 0, 255);
		grassTex.BlurGauss(0, 1);
#else
		grassTex.NormalizeCh(0x00, -12, 255);
		grassTex.BlurGauss(0, 1);
#endif
		grassTex.MkChannel(0x00);
		grassTex.ScaleColor(0, 64, 128, 32);
		//	grassTex.SaveToTGA(0, 58);

		grassTex.FractalP(0x10, 0x76, 300);
		grassTex.Diff(0x10, 0);
		grassTex.NormalizeCh(0x10, 0, 255);
		grassTex.BlurGauss(1, 2);			// halfRad = 1 gives more "groundy" look
		grassTex.Diff(0x10, 0);
		grassTex.NormalizeCh(0x10, -32, 512);
		grassTex.BlurGauss(1, 1);
		grassTex.MkChannel(0x10);
		grassTex.ScaleColor(1, 128, 80, 32);
		//	grassTex.SaveToTGA(1, 59);

		grassTex.MixMap(0, 1, 0x11, 1);
		grassTex.SaveToTGA(0, MAKE_TEX_NAME(grassTex, 0));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(leatherTex), profileCallback);

		// Leather
		leatherTex.Cells(0x30, 0x03, 24, 3, 0, 0);

		leatherTex.Cells(0x10, 0x03, 48, 4, 0, 0);
		leatherTex.Sub(0x20, 5);
		leatherTex.NormalizeCh(0x20, 32, 300);
		leatherTex.Add(0x12);
		leatherTex.Mul(3, 1);

		leatherTex.Cells(0x10, 0x03, 48, 4, 0, 0);
		leatherTex.RotoZoom(1, 0, 32);
		leatherTex.Sub(0x20, 6);
		leatherTex.NormalizeCh(0x20, 128, 255);
		leatherTex.Add(0x12);
		leatherTex.Mul(3, 1);

		leatherTex.FractalP(0x10, 0x58, 255);
		leatherTex.MapDist(3, 0x10, 0x10, 32, 32);

		leatherTex.CellsEven(0x10, 0x04, 32, 26, 0, 0);
		leatherTex.BlurGauss(1, 5, 64, 24);
		leatherTex.Contrast(1, 64, 96);
		leatherTex.InvertCh(0x10);
		leatherTex.Copy(2, leatherTex.GetLayer(1));
		leatherTex.RotoZoom(1, 0, 8);
		leatherTex.RotoZoom(2, Deg2RadMul * 90.0f, 8);
		leatherTex.Mul(1, 2);
		leatherTex.BlurGauss(1, 1, 64, 16);
		leatherTex.NormalizeCh(0x10, 128, 255);

		leatherTex.Mul(3, 1);
		leatherTex.MkChannel(0x00);
		leatherTex.Emboss(3, 3);
		leatherTex.Contrast(3, 128, 16);

		leatherTex.FractalP(0x00, 0x78, 300);
		leatherTex.ColorizeBW(0x00, 56, 0, 0, 144, 64, 0);
		leatherTex.Shade(0, 0x30);

		leatherTex.SaveToTGA(0, MAKE_TEX_NAME(leatherTex, 0));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(concreteTex0), profileCallback);

		const uint mapDistStr = 512;

		concreteTex0.FractalP(0x00, 0x68, 255);
		concreteTex0.NormalizeCh(0x00, 30, 255);
		concreteTex0.MapDist(0x00, 0x00, 0x00, mapDistStr, mapDistStr);

		concreteTex1.Copy(0, concreteTex0.GetLayer(0));

		concreteTex0.Turbulence(0, 0x00, 0x00, ((0) << 16) + 64, ((0) << 16) + 32);
		concreteTex0.MapDist(0x00, 0x00, 0x00, 0, mapDistStr);
		concreteTex0.Add(0x10);
		concreteTex0.Emboss(0, 8);
		concreteTex0.Shade(0, 0x10);
		//concreteTex0.NormalizeCh(0x00, 70, 255);
		concreteTex0.ColorizeBW(0x00, 30, 30, 30, 183, 183, 163);
		concreteTex0.SaveToTGA(0, MAKE_TEX_NAME(concreteTex0, 0));

		concreteTex0.Mix(1, 0, 0, 255);
		concreteTex0.MakeAlpha(0x00);
		concreteTex0.MakeAlpha(0x11);
		concreteTex0.NMapSobel(1, 0x13);

		concreteTex0.SaveToTGA(1, MAKE_TEX_NAME(concreteTex0, 1));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(concreteTex1), profileCallback);

		const uint mapDistStr = 512;

		concreteTex1.Turbulence(0, 0x00, 0x00, ((0) << 16) + 16, ((0) << 16) + 64);
		concreteTex1.MapDist(0x00, 0x00, 0x00, 0, mapDistStr);
		concreteTex1.Add(0x10);
		concreteTex1.Emboss(0, 8);
		concreteTex1.Shade(0, 0x10);
		//concreteTex1.NormalizeCh(0x00, 70, 255);
		concreteTex1.ColorizeBW(0x00, 30, 30, 30, 183, 183, 163);
		concreteTex1.SaveToTGA(0, MAKE_TEX_NAME(concreteTex1, 0));

		concreteTex1.Mix(1, 0, 0, 255);
		concreteTex1.MakeAlpha(0x00);
		concreteTex1.MakeAlpha(0x11);
		concreteTex1.NMapSobel(1, 0x13);

		concreteTex1.SaveToTGA(1, MAKE_TEX_NAME(concreteTex1, 1));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(concreteTex2), profileCallback);

		const uint mapDistStr = 512;

		concreteTex2.FractalP(0x00, 0x68);
		concreteTex2.NormalizeCh(0x00, 30, 255);
		concreteTex2.MapDist(0x00, 0x00, 0x00, mapDistStr, mapDistStr);
#if 1
		concreteTex2.Turbulence(0, 0x00, 0x00, ((0) << 16) + 16, ((0) << 16) + 64);
#else
		concreteTex2.Turbulence(0, 0x00, 0x00, ((0) << 16) + 64, ((0) << 16) + 32);
#endif
		concreteTex2.Add(0x10);
		concreteTex2.Emboss(0, 8);
		concreteTex2.Shade(0, 0x10);
		//concreteTex2.NormalizeCh(0x00, 70, 255);
		concreteTex2.ColorizeBW(0x00, 30, 30, 30, 183, 183, 163);

		concreteTex2.SaveToTGA(0, MAKE_TEX_NAME(concreteTex2, 0));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(bloodTex), profileCallback);

		bloodTex.Env(0, 0, 48, 0);
		bloodTex.FractalP(0x10, 0x58);
		bloodTex.Bias(0, 128, mainPregen.BiasArray);
		bloodTex.Bright(0, 255);
		bloodTex.MkChannel(0x10);
		bloodTex.Mul(0, 1);
		bloodTex.FractalP(0x10, 0x68);
		bloodTex.FractalP(0x30, 0x68);
		bloodTex.MapDist(0, 0x10, 0x30, 64, 64);

		//bloodTex.Add(0x20);
		bloodTex.Sub(0x20, 3);
		bloodTex.MapDist(0, 0x20, 0x20, 8, 12);
		bloodTex.Offset(0, -99, 38);
		bloodTex.Bias(0, 200, mainPregen.BiasArray);
		bloodTex.MakeAlpha(0x00);
		bloodTex.Balance(0x20, 180, 255);
		bloodTex.MkChannel(0x20);

		bloodTex.MakeAlpha(0x22);			// Alpha from Layer2 to Layer2
		bloodTex.NMapSobel(1, 0x23);		// Normal map from Layer2 to Layer1
		bloodTex.Xchg(0x03, 0x13);			// Copy alpha from Layer2 to Layer1
		//bloodTex.FillColor(0, 255, 255, 255);

		//bloodTex.MkChannel(0x03);

		bloodTex.SaveToTGA(0, MAKE_TEX_NAME(bloodTex, 0));
		bloodTex.SaveToTGA(1, MAKE_TEX_NAME(bloodTex, 1));
		bloodTex.SaveToTGA(2, MAKE_TEX_NAME(bloodTex, 2));
	}

	{
		uint numBloodSplatterTexs = 3;
		TexGen * bloodSplatterTex[] = { &bloodSplatter0Tex, &bloodSplatter1Tex, &bloodSplatter2Tex };

		for (uint i = 0; i < numBloodSplatterTexs; ++i)
		{
			ZoneAutoProfiler zoneProfiler(MAKE_STRING(bloodSplatterTex), profileCallback);

			if (i == 0)
			{
				srand(1350257616);
			}
			for (uint splatPoints = 0; splatPoints < 240; ++splatPoints)
			{
#if 0
				int ra = rand() % 256;
				int rx = rand() % 256;
				int rr = (rand() % 32) + 2;
				int rb = (rand() % 64) + 10;

				float ang = ra / 255.0f * _2PI;
				float rad = rx / 255.0f;
				//rad *= rad;
				rad *= 200.0f;
				int sx = rad * cosf(ang) + 128, sy = rad * sinf(ang) + 128;
#else
				int rr = (rand() % 32) + 2;
				int rb = (rand() % 64) + 10;
				int sx = rand() % 256, sy = rand() % 256;
#endif

				int testrad = rr >> 1;
				if (sx + testrad > 255 || sx - testrad < 0)
					continue;
				if (sy + testrad > 255 || sy - testrad < 0)
					continue;

				int testgrad = 127;
				if (sqr(sx - 127) + sqr(sy - 127) > testgrad*testgrad)
					continue;

				bloodSplatterTex[i]->EnvAdd(0, sx, sy, rr, rb, 0);
			}

			// Normal map
			bloodSplatterTex[i]->FractalP(0x10, 0x58);
			bloodSplatterTex[i]->Contrast(1, 128, 2);
			bloodSplatterTex[i]->MkChannel(0x10);

			bloodSplatterTex[i]->Add(0x20);
			bloodSplatterTex[i]->Contrast(2, 32, 200);
			bloodSplatterTex[i]->Mul(1, 2);

			bloodSplatterTex[i]->MakeAlpha(0x11);
			bloodSplatterTex[i]->NMapSobel(1, 0x13, false);
			//bloodSplatterTex[i]->Contrast(1, 128, 28);
			///////////////////////////////////////

			bloodSplatterTex[i]->Contrast(0, 32, 128);

			bloodSplatterTex[i]->Sub(0x20, 4);
			bloodSplatterTex[i]->DirBlur(0, 0x20, 4);

			// Normal map alpha channel
			bloodSplatterTex[i]->Mix(2, 0, 0, 255);
			bloodSplatterTex[i]->Bright(2, 29);
			bloodSplatterTex[i]->BlurGauss(2, 10, 64, 16);

			bloodSplatterTex[i]->MakeAlpha(0x22);
			bloodSplatterTex[i]->Xchg(0x23, 0x13);
			//////////////////////////////////////////////////////////////////////////

			bloodSplatterTex[i]->MakeAlpha(0x00);

			bloodSplatterTex[i]->FractalP(0x00, 0x58);
			bloodSplatterTex[i]->Contrast(0, 128, 10);
			bloodSplatterTex[i]->MkChannel(0x00);

			bloodSplatterTex[i]->ColorizeBW(0x00, 0, 0, 0, 128, 24, 8);

			bloodSplatterTex[i]->BlendCh(0x00, 0x03, TexGen::BlendMode::MUL);

			const int bloodSplatterTexNameSize = 256;
			char bloodSplatterTexName[bloodSplatterTexNameSize];
			sprintf_s(bloodSplatterTexName, bloodSplatterTexNameSize, TEX_FOLDER "bloodSplatterTex%d_0.tga", i);
			bloodSplatterTex[i]->SaveToTGA(0, bloodSplatterTexName);
			sprintf_s(bloodSplatterTexName, bloodSplatterTexNameSize, TEX_FOLDER "bloodSplatterTex%d_1.tga", i);
			bloodSplatterTex[i]->SaveToTGA(1, bloodSplatterTexName);
			sprintf_s(bloodSplatterTexName, bloodSplatterTexNameSize, TEX_FOLDER "bloodSplatterTex%d_2.tga", i);
			bloodSplatterTex[i]->SaveToTGA(2, bloodSplatterTexName);
		}
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(bulletHoleTex), profileCallback);

		bulletHoleTex.Env(0, 0, 30, 0);
		bulletHoleTex.Contrast(0, 128, 40);
		//	bulletHoleTex.FractalP(0x20, 0x38);
		bulletHoleTex.Sub(0x20, 3);
		bulletHoleTex.MapDist(0, 0x20, 0x20, 3, 4);
		bulletHoleTex.MakeAlpha(0x00);
		bulletHoleTex.MakeAlpha(0x10);
		bulletHoleTex.Add(0x40);
		bulletHoleTex.MakeAlpha(0x44);
		bulletHoleTex.NMapSobel(1, 0x43);
		//	bulletHoleTex.Invert_RGB(0);
		bulletHoleTex.Contrast(0, 180, 68);
		bulletHoleTex.MakeAlpha(0x00);
		bulletHoleTex.FillColor(0, 0, 0, 0);

		bulletHoleTex.SaveToTGA(0, MAKE_TEX_NAME(bulletHoleTex, 0));
		bulletHoleTex.SaveToTGA(1, MAKE_TEX_NAME(bulletHoleTex, 1));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(polishedMetalTex), profileCallback);

		{
			const uint numColors = 512;
			int colors[numColors];

			// num lead colors : 12
			const uint numLeadColors = 12;

			// Should be symmetric
			int leadColors[numLeadColors] = { 192, 255, 200, 90, 192, 110,
				192, 255, 200, 90, 192, 110 };

			float spacePerColor = numColors / (float)numLeadColors;

#if 0
			// Polished metal LERP
			for (uint i = 0; i < numColors; ++i)
			{
				float fpCurLeadColor = i / spacePerColor;
				int idxCurLeadColor = (int)fpCurLeadColor;
				float fract = fpCurLeadColor - idxCurLeadColor;
				int idxCurLeadColor1 = idxCurLeadColor + 1;
				if (idxCurLeadColor1 >= numLeadColors)
				{
					idxCurLeadColor1 = 0;
				}

				colors[i] = leadColors[idxCurLeadColor] * (1.f - fract) + leadColors[idxCurLeadColor1] * fract;
			}
#else
			// Polished metal Catmull-Rom
			for (uint i = 0; i < numColors; ++i)
			{
				// Desired interpolation value is between p1 and p2:
				// p0-----p1-*---p2-----p3

				float fpCurLeadColor = i / spacePerColor;
				int idxCurLeadColor1 = (int)fpCurLeadColor;
				float fract = fpCurLeadColor - idxCurLeadColor1;

				int idxCurLeadColor0 = idxCurLeadColor1 - 1;
				if (idxCurLeadColor0 < 0)
				{
					idxCurLeadColor0 = numLeadColors - 1;
				}

				int idxCurLeadColor2 = idxCurLeadColor1 + 1;
				if (idxCurLeadColor2 >= numLeadColors)
				{
					idxCurLeadColor2 = 0;
				}

				int idxCurLeadColor3 = idxCurLeadColor2 + 1;
				if (idxCurLeadColor3 >= numLeadColors)
				{
					idxCurLeadColor3 = 0;
				}

				const float t0 = 1.0f;
				float t = fract;
				float t2 = t*t;
				float t3 = t2*t;

				float p0 = (float)leadColors[idxCurLeadColor0];
				float p1 = (float)leadColors[idxCurLeadColor1];
				float p2 = (float)leadColors[idxCurLeadColor2];
				float p3 = (float)leadColors[idxCurLeadColor3];


				float catmRomInterp =
					(       2*p1            ) * t0 +
					( -p0 +          p2     ) * t  +
					(2*p0 - 5*p1 + 4*p2 - p3) * t2 +
					( -p0 + 3*p1 - 3*p2 + p3) * t3;

				colors[i] = (int)(0.5f * catmRomInterp);
			}
#endif

			polishedMetalTex.RadGrad(0x00, 255, 255, colors, numColors);
			polishedMetalTex.MkChannel(0x00);
			polishedMetalTex.SaveToTGA(0, MAKE_TEX_NAME(polishedMetalTex, 0));
		}

		polishedMetalTex.PointsWhite(1, 256);
		polishedMetalTex.RotoZoomMul(1, 6, Deg2RadMul * 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 255, 255, 255);
		polishedMetalTex.Offset(1, -108, -108);

		polishedMetalTex.Add(0x21);
		polishedMetalTex.RotoZoom(2, Deg2RadMul * 90.0f, 64);
		polishedMetalTex.Offset(2,  138,  68);

		polishedMetalTex.Add(0x31);
		polishedMetalTex.RotoZoom(3, Deg2RadMul * 180.0f, 64);
		polishedMetalTex.Offset(3,  38,  138);

		polishedMetalTex.Add(0x12);
		polishedMetalTex.Add(0x13);

		polishedMetalTex.SaveToTGA(1, MAKE_TEX_NAME(polishedMetalTex, 1));

		polishedMetalTex.Bright(1, 8);

		polishedMetalTex.Invert_RGB(1);
		polishedMetalTex.Emboss(1, 3);
		polishedMetalTex.RotoZoom(1, 0, 32);

		polishedMetalTex.Shade(0, 0x10);

		polishedMetalTex.SaveToTGA(0, MAKE_TEX_NAME(polishedMetalTex, 2));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(rustyTex), profileCallback);

		// Adding fractal pattern to the mask layer
		rustyTex.FractalP(0x40, 0x68, 256, 0);
		rustyTex.Bright512(4, 1200);
		rustyTex.SineColor(0x40, 1.5f);
		rustyTex.InvertCh(0x40);

		rustyTex.MkChannel(0x40);

		// Adding dots/stains pattern to the mask layer
		for (uint splatPoints = 0; splatPoints < 64; ++splatPoints)
		{
			int rr = (rand() % 4) + 6;
			int rb = 512;
			int sx = rand() % 1024, sy = rand() % 1024;

			rustyTex.EnvAdd(4, sx, sy, rr, rb, 0);
		}

		// Distorting mask layer
		rustyTex.FractalP(0x10, 0x48, 256);
		rustyTex.MapDist(4, 0x10, 0x10, 16, 16);

#define RUSTYTEXT 0

		//	rustyTex.FillColor(4, 0, 0, 0);
#if (RUSTYTEXT == 1)
		rustyTex.InitFont("Arial", 100, (1<<2) + (0<<1) + 0);
		rustyTex.RenderFont(7, 0, 520, "Rusty text");
		rustyTex.MapDist(7, 0x10, 0x10, 15, 15, 128, 128);
		rustyTex.Blur(7, 2);

		//rustyTex.Bright(4, 16);

		// Adding text to the mask layer
		rustyTex.Add(0x47);
#endif

		// Generating rusty stains/runs
		rustyTex.FillColor(1, 0, 0, 0);
		rustyTex.Add(0x14);
		rustyTex.Bright512(1, 40);
		//	rustyTex.Rect(0x00, 0, 128, 256, 4, 255, 0);
		rustyTex.RotoZoomMul(1, 4, 0.0f, 1.0f, 1.0f, 0.0f, 0.01f, 250, 250, 250);
		//	rustyTex.RotoZoomMul(1, 4, 0.0f, 1.0f, 1.0f, 0.0f, 0.007f, 250, 250, 250);

		// Distorting stains/run
		rustyTex.FractalP(0x60, 0x48, 256);
		rustyTex.MapDist(1, 0x60, 0x60, 12, 12, 128, 128);

		// Adding stains/runs to the mask layer
		rustyTex.Bright512(4, 256);
		rustyTex.Add(0x41);
		//rustyTex.NormalizeCh(0x40, 0, 220);

		// Rust Layer
		// Generate rusty background
		rustyTex.FractalP(0x20, 0x58, 256, 0);
		rustyTex.ColorizeBW(0x20, 40, 0, 11, 114, 45, 22);

		// Generate rusty dots
		rustyTex.Random(0x30, 255);
		rustyTex.NormalizeCh(0x30, -4096, 255);
		rustyTex.Blur(3, 2);
#if 1
		// Just add dots
		rustyTex.ColorizeBW(0x30, 0, 0, 0, 102, 110, 45);
		rustyTex.Add(0x23);
#else
		// Emulate lighting
		rustyTex.Emboss(3, 4);
		rustyTex.Shade(2, 0x30);
#endif

		// Clear (target) layer
		rustyTex.FractalP(0x00, 0x56, 256);
		rustyTex.ColorizeBW(0x00, 40, 80, 10, 45, 90, 15);

		// Generate dots for the clear layer
		rustyTex.Random(0x10, 255);
		rustyTex.NormalizeCh(0x10, -4096, 255);
		rustyTex.Blur(1, 1);
#if 0
		// Simply add dots
		rustyTex.ColorizeBW(0x10, 0, 0, 0, 51, 55, 27);
		rustyTex.Add(0x01);
#else
		// Emulate lighting
		// Later this layer will be added to the mask layer as well, producing interesting effect
		rustyTex.Emboss(1, 3);
		rustyTex.Shade(0, 0x11);
		rustyTex.Add(0x01);
#endif

		//rustyTex.Shade(0, 0x50);

#if (RUSTYTEXT == 1)
		rustyTex.Invert_RGB(7);
		rustyTex.Emboss(7, 10);
		rustyTex.Shade(0, 0x70);
#endif

		rustyTex.Add(0x41);
		//rustyTex.Contrast(4, 128, 12);

		rustyTex.MixMap(0, 2, 0x40, 1);

		rustyTex.Offset(0, 258, 20);

		rustyTex.SaveToTGA(0, MAKE_TEX_NAME(rustyTex, 0));

#undef RUSTYTEXT
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(lensFlareDirtTex), profileCallback);

		// Lens flare dirt tex
		const uint lfDirtSizeX = lensFlareDirtTex.GetWidth();
		const uint lfDirtSizeY = lensFlareDirtTex.GetHeight();
		const uint lfDirtHalfSize = lfDirtSizeX / 2;
		for (uint splatPoints = 0; splatPoints < 100; ++splatPoints)
		{
			int rr = (rand() % 64) + 2;
			int rb = (rand() % 128) + 10;
			int sx = rand() % lfDirtSizeX, sy = rand() % lfDirtSizeY;

			lensFlareDirtTex.EnvAdd(0, sx, sy, rr, rb, 0);
		}
		lensFlareDirtTex.Contrast(0, 128, 50);
		for (uint splatPoints = 0; splatPoints < 200; ++splatPoints)
		{
			int rr = (rand() % 32) + 2;
			int rb = (rand() % 128) + 10;
			int sx = rand() % lfDirtSizeX, sy = rand() % lfDirtSizeY;

			lensFlareDirtTex.EnvAdd(0, sx, sy, rr, rb, 0);
		}
		//lensFlareDirtTex.Contrast(0, 128, 8);

		lensFlareDirtTex.FractalP(0x10, 0x58);
		lensFlareDirtTex.MkChannel(0x10);
		lensFlareDirtTex.Contrast(1, 128, 8);
		lensFlareDirtTex.Add(0x01);

		lensFlareDirtTex.Contrast(0, 128, 4);
		lensFlareDirtTex.Bright(0, 56);

		lensFlareDirtTex.SaveToTGA(0, MAKE_TEX_NAME(lensFlareDirtTex, 0));
	}

	{
		ZoneAutoProfiler zoneProfiler(MAKE_STRING(dustTex), profileCallback);

		dustTex.FractalP(0x00, 0x38);
		dustTex.Env(1, 1, 40, 0);
		dustTex.FractalP(0x20, 0x38);
		dustTex.MapDist(1, 0x20, 0x00, 24, 24, 128, 128);
		dustTex.Mul(0, 1);
		dustTex.MkChannel(0x00);
		dustTex.MakeAlpha(0x00);
		dustTex.Bright(0, 64);

		dustTex.SaveToTGA(0, MAKE_TEX_NAME(dustTex, 0));
		dustTex.SaveToTGA(1, MAKE_TEX_NAME(dustTex, 1));
	}

#undef MAKE_STRING
#undef MAKE_TEX_NAME
#undef TEX_FOLDER

	printf("Finished execution\n");
	getch();

	return 0;
}