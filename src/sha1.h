#include<vector>
#include<array>
#include<netinet/in.h>
#define LittleEndian

class SHA1
{
public:
	template<class It> std::array<unsigned char, 20> hash(const It begin, const It end)
	{
		for(int i=0; i<5; i++) h[i] = h_stored_value[i];
		std::vector<unsigned char> v(begin, end);
		preprocess(v);
		for(int i=0; i<v.size(); i+=64) process_chunk(&v[i]);
		uint32_t r[5];
		for(int i=0; i<5; i++) r[i] = htonl(h[i]);
		std::array<unsigned char, 20> ar; unsigned char *p = (unsigned char*)r;
		for(int i=0; i<20; i++) ar[i] = *p++;
		return ar;
	}

protected:
	uint32_t h[5];
	static constexpr uint32_t h_stored_value[5] =
		{0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

private:
	void preprocess(std::vector<unsigned char> &v);
	void process_chunk(unsigned char *p);//64 byte chunk
	uint32_t left_rotate(uint32_t a, int bits);
};
