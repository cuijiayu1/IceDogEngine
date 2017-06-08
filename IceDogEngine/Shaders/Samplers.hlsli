

SamplerState linearSample : register(s0);
/*{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
*/

SamplerState samAnisotropic : register(s1);
/*{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};
*/

SamplerState shadowSample : register(s2);
/*{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};
*/
SamplerState pointSample : register(s3);