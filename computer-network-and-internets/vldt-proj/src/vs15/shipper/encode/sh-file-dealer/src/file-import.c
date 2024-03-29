<<<<<<< HEAD:src/vs15/shipper/encode/sh-file-dealer/src/file-import.c
// file-import.c   29/03/2020
// J_Code
// @brief: 编码器文件导入机

// 输入数据，并转换为一个J_Data数组 
=======
// 杈撳叆鏁版嵁锛屽苟杞崲涓轰竴涓狫_Data鏁扮粍 
>>>>>>> 0e4b4c744c880271eee04c60c95c8187093a2fff:src/encoder/file_dealer/file-import.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jc.h>
#include "dll.h"

<<<<<<< HEAD:src/vs15/shipper/encode/sh-file-dealer/src/file-import.c
// 构建filePack 
SH_Pack* fileImport(const char filePath[], int desiredTime_ms) {
=======
// 鏋勫缓filePack 
SH_Pack* fileImport(const char filePath[]) {
>>>>>>> 0e4b4c744c880271eee04c60c95c8187093a2fff:src/encoder/file_dealer/file-import.c
	int getFileInfoStatus, i;
	SH_FileInfo fileinfo;
	getFileInfo(&fileinfo, filePath, &getFileInfoStatus);
	if (getFileInfoStatus < 0) {
		printf("SH_ERROR: Import Error - No such file or directory.\n");
		return NULL;
	}

	// 分析：该文件是否能够被完整输入
	int maxData = (desiredTime_ms / 1000 * MAX_FRAME_PER_SECOND - 3) * MAXIMUM_FILE_SIZE;
	if (fileinfo.file_size > maxData) {
		// 该文件太大，必须被截取掉一部分
		printf_s("SH_WARNING: The preferred video length is not enough, which provides capacity of only %d Bytes, while the inputed file sizes %d Bytes. The over flowed data will be diminished.\n", maxData, fileinfo.file_size);
		fileinfo.file_size = maxData;
	}
	
	// 寮曞叆鎴愬姛锛屾瀽鍙栨枃浠朵俊鎭紝缂栧啓绗竴涓暟鎹寘
	J_Data* filepackHead = (J_Data *)malloc(sizeof(J_Data) + 1 + sizeof(fileinfo));
	if (!filepackHead) {
		return NULL;
	}
	filepackHead->length = sizeof(int) + sizeof(fileinfo);
	int* pack_num_buff = (int *)filepackHead->data;	
	*pack_num_buff = 0;	// 棣栧抚锛�
	memcpy(filepackHead->data+sizeof(int), &fileinfo, sizeof(fileinfo));
	
	int pack_mod =  fileinfo.file_size % MAXIMUM_FILE_SIZE;		// 鏈�鍚庝竴甯х殑闀垮害 
	int pack_int =  fileinfo.file_size / MAXIMUM_FILE_SIZE;
	int pack_num =  pack_int + 
					(pack_mod ? 1 : 0);
	
<<<<<<< HEAD:src/vs15/shipper/encode/sh-file-dealer/src/file-import.c
	// 现在获取文件包 
	SH_Pack* newPack = (SH_Pack*)malloc(sizeof(SH_Pack) + sizeof(J_Data*) * (pack_num + 1));
=======
	// 鐜板湪鑾峰彇鏂囦欢鍖� 
	SH_Pack* newPack = (SH_Pack*)malloc(sizeof(SH_Pack) + sizeof(J_Data*) * (pack_num + sizeof(int)));
>>>>>>> 0e4b4c744c880271eee04c60c95c8187093a2fff:src/encoder/file_dealer/file-import.c
	if (!newPack) {
		free(filepackHead);
		return NULL;
	}
	newPack->packs = 1;
	newPack->data[0] = filepackHead;
<<<<<<< HEAD:src/vs15/shipper/encode/sh-file-dealer/src/file-import.c
	// 现在写入文件包
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath, "rb");
	if (err) {
=======
	// 鐜板湪鍐欏叆鏂囦欢鍖�
	FILE* fp = fopen(filePath, "rb");
	if (!fp) {
>>>>>>> 0e4b4c744c880271eee04c60c95c8187093a2fff:src/encoder/file_dealer/file-import.c
		free(filepackHead);
		free(newPack);
		return NULL;
	}
	for (i = 0; i < pack_num - 1; i++) {
		J_Data* filepack = (J_Data *)malloc(sizeof(J_Data) + sizeof(int) + MAXIMUM_FILE_SIZE);	// unsafe
		filepack->length = sizeof(int) + MAXIMUM_FILE_SIZE;
		pack_num_buff = (int *)filepack->data;
		*pack_num_buff = i + 1; // 鍐欏叆甯х紪鍙� 
		fread(filepack->data + sizeof(int), MAXIMUM_FILE_SIZE, 1, fp);
		// 姹囧叆鍖�
		newPack->data[i + 1] = filepack;
		newPack->packs++;
	}
	// 鐜板湪鍐欏叆灏惧寘
	if (pack_mod) {
		J_Data* filepack = (J_Data *)malloc(sizeof(J_Data) + sizeof(int) + pack_mod);			// unsafe
		filepack->length = sizeof(int) + pack_mod;
		pack_num_buff = (int *)filepack->data;
		*pack_num_buff = pack_num;
		fread(filepack->data + sizeof(int), pack_mod, 1, fp);
		// 姹囧叆鍖�
		newPack->data[pack_num] = filepack;
	} else {
		J_Data* filepack = (J_Data *)malloc(sizeof(J_Data) + sizeof(int) + MAXIMUM_FILE_SIZE);	// unsafe
		filepack->length = sizeof(int) + MAXIMUM_FILE_SIZE;
		pack_num_buff = (int *)filepack->data;
		*pack_num_buff = pack_num;
		fread(filepack->data + sizeof(int), MAXIMUM_FILE_SIZE, 1, fp);
		// 姹囧叆鍖�
		newPack->data[pack_num] = filepack;
	}
	newPack->packs++;
	// 缁撴潫 
	fclose(fp);
	/*
	printf("File Size: %d\n", fileinfo.file_size);
	printf("Written: %d packs\n" , pack_num);
	printf("Mod: %d\n", pack_mod);
	*/
	return newPack;
}

// 閿�姣乫ilePack 
void destroyFilePack(SH_Pack* pack) {
	// free鎺夊叾涓墍鏈塉_Data
	int i = 0;
	for (i = 0; i < pack->packs; i++) {
		free(pack->data[i]);
	}
	// free鎺塸ack
	free(pack); 
}
