#include<iostream>
#include<string>
#include<array>
class KeyGenerator {
    std::array<int, 4> armstrongNumbers = { 153, 370, 371, 407 };
    std::array<int, 24> baseTable = { 1234, 1243, 1324, 1342, 1423, 1432, 2134, 2143, 2314, 2341, 2413, 2431, 3124,
            3142, 3214, 3241, 3412, 3421, 4123, 4132, 4213, 4231, 4312, 4321 };

    std::string key, numericKey;

public:
    KeyGenerator(std::string k) {
        // this reference of Java == self reference of Python
        // this is implicitly declared and used.
        key = k;
        numericKey = "";
    }
    std::string getNumericKey() {
        if (numericKey == "")
            generateNumericKey();

        return numericKey;
    }

    void generateNumericKey() {// key: How old is my computer?
        int tot = 0;
        int l, i;
        l = key.length();
        for (i = 0; i < l; i++) {// add the ASCII of characters of key

            tot += key[i];
        }
        // example : tot = 2141

        int permutation = baseTable[tot % (baseTable.size())];
        // example : permutation = baseTable[5] = 1432

        std::string temp = "";
        std::cout << "*" << permutation << '\n';
        while (permutation > 0) {
            temp = std::to_string(armstrongNumbers[permutation % 10 - 1]) + temp;
            permutation /= 10;
        }
        // numericKey = partA+partB
        numericKey = temp + std::to_string(tot);
    }

    void display() {
        std::cout << "key : " << key << "\n";
        std::cout << "numeric key : " << numericKey << "\n";
    }
};

class ArmstrongManager {
    std::string numericKey;
    int enc_index, dec_index;
public:
    ArmstrongManager(std::string nK) {
        numericKey = nK;
        enc_index = -1;
        dec_index = -1;
    }

    int encrypt(int data) {
        try {
            return data ^ numericKey[++enc_index];
        }
        catch (int ex) {
            enc_index = -1;
            return data ^ numericKey[++enc_index];
        }
    }


    int decrypt(int data)
    {
        try
        {
            return data ^ numericKey[++dec_index];
        }
        catch (int ex)
        {
            dec_index = -1;
            return data ^ numericKey[++dec_index];
        }
    }
};

class ByteManager {
public:
    static int byteToNibble(int x, int arr[]) {
        // ‭104‬ ---> ‭[0110,1000‬]
        arr[0] = (x & 255) >> 4;// higher nibble
        arr[1] = x & 15;// lower nibble
    }

    static int nibblesToByte(int nibbles[]) {
        // ‭[0110,1000‬] ---> 104
        return ((nibbles[0] & 15) << 4) | (nibbles[1] & 15);
    }
};



class ColorManager {
    int* rgb;
    int enc_index, dec_index;
public:
    ColorManager(std::string nK) {
        rgb = new int[3];
        enc_index = dec_index = -1;

        int partB = std::stoi(nK.substr(12));               // int(nK[12:])
        rgb[0] = (std::stoi(nK.substr(0, 4)) + partB) % 256; // int(nK[0:4])
        rgb[1] = (std::stoi(nK.substr(4, 8)) + partB) % 256; // int(nK[4:8])
        rgb[2] = (std::stoi(nK.substr(8, 12)) + partB) % 256; // int(nK[8:12])
    }

    int encrypt(int data) {
        int nibbles[2];
        ByteManager b;
        b.byteToNibble(data, nibbles);
        enc_index = (enc_index + 1) % 3;
        return (rgb[enc_index] + nibbles[0] * 16 + nibbles[1]) % 256;
    }

    int decrypt(int data) {
        int temp;
        int nibbles[2];
        dec_index = (dec_index + 1) % 3;
        temp = (data - rgb[dec_index] + 256) % 256;

        nibbles[0] = temp / 16;
        nibbles[1] = temp % 16;
        ByteManager b;
        return b.nibblesToByte(nibbles);

    }
};

int main() {
    int a;
    std::string data = "This is an important statement.";
    std::string key = "How old is my computer?";
    KeyGenerator kGen(key);
    std::string nK = kGen.getNumericKey();
    //DEBUG
    kGen.display();
    ArmstrongManager aMgr(nK);
    ColorManager cMgr(nK);

    std::string encData = "";
    int temp;
    int i;
    for (i = 0; i < data.length(); i++) {
        temp = aMgr.encrypt(data[i]);
        temp = cMgr.encrypt(temp);
        encData = encData + (char)temp;
    }

    std::string decData = "";
    for (i = 0; i < encData.length(); i++) {
        temp = cMgr.decrypt(encData[i]);
        temp = aMgr.decrypt(temp);

        decData = decData + (char)temp;
    }

    std::cout << "data: " << data << "  " << data.length() << "\n";
    std::cout << "enc data: " << encData << "  " << encData.length() << "\n";
    std::cout << "dec data: " << decData << "  " << decData.length() << "\n";


}