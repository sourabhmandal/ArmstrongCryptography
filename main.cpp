#include<iostream>
//#include"encode.h"
//#include"decode.h"
#include<opencv2/opencv.hpp>
#include<cstdint>
#include<bitset>
#include<vector>

class Encode {
	char* secret_key;
	std::bitset<8> message_bits;
	std::bitset<8> image_bits;
	char* msg;
public:
	Encode(char key[], char message[]) {
		secret_key = key;
		msg = message;

	}

	void enctypt_message() {
		//XOR encrytion
		int i = 0, j=0;
		int secret_key_len = strlen(secret_key);
		int msg_len = strlen(msg);
		for (int i = 0; i < secret_key_len; i++) {
			int a = secret_key[i];
			//std::cout << "ASCII : " << a << std::endl;
		}
		std::cout << secret_key_len;
		for (int i = 0; i < msg_len; i++) {
			msg[i] ^= secret_key[j % secret_key_len];
			i++;
			j++;
		}
	}

	void string_to_bitset_array(cv::Mat &img) {
		int msg_i = 0;
		std::cout << "Image :\n";
		
		for (int i = 0; i < img.cols; i++) {
			for (int j = 0; j < img.rows; j++) {
				if (msg_i == strlen(msg)) return;
				if (j + 4 > img.rows) i++;
				//access each pixel of green channel
				int pixel = img.at<cv::Vec3b>(j, i)[1];
				//breaking the bits of each message charachter into 2 bit pairs
				int bits;
				bits = msg[msg_i] & 3;
				//discard last 2 bits
				msg[msg_i] >> 2;
				pixel >> 2;
				//append last 2 bits
				pixel |= bits;
				if(j%4 == 0 && j>0)msg_i++;

			}
		}
	}
};


// main function
// required args = image_file, message_text
int main(int argc, char** argv) {
	cv::Mat img = cv::imread("lina.png", cv::IMREAD_UNCHANGED);
	int height = img.rows;
	int width = img.cols;
	int channels = img.channels();
	std::cout << "Height : " << height << " Width : " << width << " Channels : "<< channels <<"\n";
	
	//images are in bgr format
	//embedding data in last two significant bits of blue channel
	
	
	char message[] = "ABCDE";
	char secret[] = "Secret";

	Encode e(secret, message);
	e.enctypt_message();
	e.string_to_bitset_array(img);
	return 0;
}