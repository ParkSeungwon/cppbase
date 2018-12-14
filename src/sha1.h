#include<vector>
#include<array>

class SHA1
{
public:
	template<class It> std::array<unsigned char, 20> hash(It begin, It end) {
		std::vector<unsigned char> v(begin, end);
		preprocess(v);

	}

protected:
	uint8_t h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

private:
	void preprocess(std::vector<unsigned char> &v);
	void process_chunk(unsigned char *p);//64 byte chunk
	uint32_t left_rotate(uint32_t k, int bits);
};
