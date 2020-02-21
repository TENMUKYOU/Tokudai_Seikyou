#ifndef _FELICALIB_H
#define        _FELICALIB_H

#include <windows.h>
#include <tchar.h>

typedef unsigned char uint8;
typedef unsigned short int uint16;

typedef struct strpasori pasori;

#define MAX_SYSTEM_CODE         8
#define MAX_AREA_CODE           16
#define MAX_SERVICE_CODE        256

/**
   @brief FeliCa ハンドル
   FeliCa の情報を格納する構造体
*/
typedef struct strfelica {
    pasori *p;          /**< PaSoRi ハンドル */
    uint16 systemcode;  /**< システムコード */
    uint8 IDm[8];       /**< IDm */
    uint8 PMm[8];       /**< PMm */

    /* systemcode */
    uint8 num_system_code;                      /**< 列挙システムコード数 */
    uint16 system_code[MAX_SYSTEM_CODE];        /**< 列挙システムコード */

    /* area/service codes */
    uint8 num_area_code;                        /**< エリアコード数 */
    uint16 area_code[MAX_AREA_CODE];            /**< エリアコード */
    uint16 end_service_code[MAX_AREA_CODE];     /**< エンドサービスコード */

    uint8 num_service_code;                     /**< サービスコード数 */
    uint16 service_code[MAX_SERVICE_CODE];      /**< サービスコード */
} felica;

/* constants */
/* システムコード (ネットワークバイトオーダ/ビックエンディアンで表記) */
#define POLLING_ANY     0xffff
#define POLLING_EDY     0xfe00  /**< システムコード: 共通領域 (Edy などが使用) */
#define POLLING_SUICA   0x0003  /**< システムコード: サイバネ領域 */

/* endian */
/** バイトオーダ変換(16bit) */
#define        SW2B(x)          (((x) >> 8) & 0xff | ((x) << 8) & 0xff00)
/** ネットワークバイトオーダからホストバイトオーダへの変換(16bit) */
#define        N2HS(x)          SW2B(x)
/** ホストバイトオーダからネットワークバイトオーダへの変換(16bit) */
#define        H2NS(x)          SW2B(x)

/* APIs */
#ifdef __cplusplus
extern "C" {
#endif

pasori *pasori_open(char *);
pasori *pasori_open2(TCHAR *);
void pasori_close(pasori *);

int pasori_init(pasori *);
felica* felica_polling(pasori *, uint16, uint8, uint8);
void felica_free(felica *f);
void felica_getidm(felica *f, uint8 *buf);
void felica_getpmm(felica *f, uint8 *buf);

int felica_read_without_encryption02(felica *f, int servicecode, int mode, uint8 addr, uint8 *b);

int felica_write_without_encryption(felica *f, int servicecode, uint8 addr, uint8 *b); // WARNING! NOT TESTED!!

felica * felica_enum_systemcode(pasori *p);
felica * felica_enum_service(pasori *p, uint16 systemcode);

#ifdef __cplusplus
}
#endif

#endif /* _FELICALIB_H */