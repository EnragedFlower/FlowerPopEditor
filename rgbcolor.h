#pragma once

struct rgbcolor {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	inline std::string to_string() {
		return "r" + std::to_string(r) + "g" + std::to_string(g) + "b" + std::to_string(b);
	}

	inline bool equals(const uint8_t& cr, const uint8_t& cg, const uint8_t& cb) {
		return r == cr && g == cg && b == cb;
	}

	inline bool operator==(const rgbcolor& color) const {
		return r == color.r && g == color.g && b == color.b;
	}
	inline bool operator!=(const rgbcolor& color) const {
		return !(*this == color);
	}
};

template<>
struct std::hash<rgbcolor> {
	inline std::size_t operator()(const rgbcolor& c) const {
		std::size_t h1 = std::hash<uint8_t>{}(c.r);
		std::size_t h2 = std::hash<uint8_t>{}(c.g);
		std::size_t h3 = std::hash<uint8_t>{}(c.b);
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};
