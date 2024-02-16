#include <inttypes.h>
#include <stdio.h>

#include "../../src/poseidongold/poseidongold_element.h"
#include "../../src/poseidongold/poseidongold_hash.h"

int main() {
#ifdef __AVX512F__
    printf("Using AVX512\n");
#elif __AVX2__
    printf("Using AVX2\n");
#else
    printf("Using scalars\n");
#endif

    const size_t size = 8;

    uint64_t input[][8] = {
        {5577006791947779410ULL, 8674665223082153551ULL, 15352856648520921629ULL, 13260572831089785859ULL, 3916589616287113937ULL, 6334824724549167320ULL, 9828766684487745566ULL, 10667007354186551956ULL},
        {894385949183117216ULL, 11998794077335055257ULL, 4751997750760398084ULL, 7504504064263669287ULL, 11199607447739267382ULL, 3510942875414458836ULL, 12156940908066221323ULL, 4324745483838182873ULL},
        {11833901312327420776ULL, 11926759511765359899ULL, 6263450610539110790ULL, 11239168150708129139ULL, 1874068156324778273ULL, 3328451335138149956ULL, 14486903973548550719ULL, 7955079406183515637ULL},
        {11926873763676642186ULL, 2740103009342231109ULL, 6941261091797652072ULL, 1905388747193831650ULL, 17204678798284737396ULL, 15649472107743074779ULL, 4831389563158288344ULL, 261049867304784443ULL},
        {10683692646452562431ULL, 5600924393587988459ULL, 18218388313430417611ULL, 9956202364908137547ULL, 5486140987150761883ULL, 9768663798983814715ULL, 6382800227808658932ULL, 2781055864473387780ULL},
        {10821471013040158923ULL, 4990765271833742716ULL, 14242321332569825828ULL, 11792151447964398879ULL, 13126262220165910460ULL, 14117161486975057715ULL, 2338498362660772719ULL, 2601737961087659062ULL},
        {7273596521315663110ULL, 3337066551442961397ULL, 17344948852394588913ULL, 11963748953446345529ULL, 8249030965139585917ULL, 898860202204764712ULL, 9010467728050264449ULL, 9908585559158765387ULL},
        {11273630029763932141ULL, 15505210698284655633ULL, 2227583514184312746ULL, 12096659438561119542ULL, 8603989663476771718ULL, 6842348953158377901ULL, 7388428680384065704ULL, 6735196588112087610ULL},
    };
    uint64_t capacity[][4] = {
        {0ULL, 0ULL, 0ULL, 0ULL}, {0ULL, 0ULL, 0ULL, 0ULL}, {0ULL, 0ULL, 0ULL, 0ULL}, {0ULL, 0ULL, 0ULL, 0ULL}, {0ULL, 0ULL, 0ULL, 0ULL}, {0ULL, 0ULL, 0ULL, 0ULL}, {0ULL, 0ULL, 0ULL, 0ULL}, {0ULL, 0ULL, 0ULL, 0ULL},
    };
    uint64_t expectedResult[][4] = {
        {7986352640330579808ULL, 16698976638447200418ULL, 14099060853601989680ULL, 1806029100513259151ULL}, {4912038160490892692ULL, 4103686885524875147ULL, 10541378107520313959ULL, 17279065624757782690ULL},
        {7890399244011379224ULL, 3893786354640587971ULL, 13560196409483468805ULL, 2948784903943663078ULL},  {14700294369684741534ULL, 14895735373969203815ULL, 16434826207003907392ULL, 17867437290801947189ULL},
        {17625602373883346164ULL, 18149609703473926001ULL, 2824810185344003270ULL, 9975387089464755098ULL}, {2691796281516679790ULL, 14584425213549820217ULL, 14318483276994184927ULL, 17940735015359233298ULL},
        {8150678766998536923ULL, 14721645535435562842ULL, 2012097115710913290ULL, 6143064933387483688ULL},  {9286804905202849659ULL, 7450030268744143082ULL, 2587697138684996149ULL, 603260420412321806ULL},
    };

    uint64_t result[4];

    for (int i = 0; i < size; ++i) {
#ifdef __AVX512F__
        PoseidongoldElement goldilocksInput[24];
        PoseidongoldElement goldilocksResult[24];
        memcpy(goldilocksInput, input[i], 4 * sizeof(unsigned long long));
        memcpy(goldilocksInput + 8, input[i] + 4, 4 * sizeof(unsigned long long));
        memcpy(goldilocksInput + 16, capacity[i], 4 * sizeof(unsigned long long));
        memset(goldilocksResult, 0, 24 * sizeof(unsigned long long));
#else
        PoseidongoldElement goldilocksInput[12];
        PoseidongoldElement goldilocksResult[12];
        memcpy(goldilocksInput, input[i], 8 * sizeof(unsigned long long));
        memcpy(goldilocksInput + 8, capacity[i], 4 * sizeof(unsigned long long));
        memset(goldilocksResult, 0, 12 * sizeof(unsigned long long));
#endif

#ifdef __AVX512F__
        PoseidongoldHash_result_avx512(goldilocksResult, goldilocksInput);
#elif __AVX2__
        PoseidongoldHash_result_avx2(goldilocksResult, goldilocksInput);
#else
        PoseidongoldHash_result_scalar(goldilocksResult, goldilocksInput);
#endif

        result[0] = PoseidongoldElement_toU64(goldilocksResult[0]);
        result[1] = PoseidongoldElement_toU64(goldilocksResult[1]);
        result[2] = PoseidongoldElement_toU64(goldilocksResult[2]);
        result[3] = PoseidongoldElement_toU64(goldilocksResult[3]);

        for (int j = 0; j < 4; ++j) {
            if (expectedResult[i][j] != result[j]) {
                printf("Wrong case %d\n", i);
                printf("%lu %lu %lu %lu\n", result[0], result[1], result[2], result[3]);
                return 1;
            }
        }
    }
    printf("DONE\n");

    return 0;
}
