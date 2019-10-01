all:
	g++ main.cpp -o huffman

clean:
	rm -vrf ./~* ./huffman ./*.key
