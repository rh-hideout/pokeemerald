#! /bin/zsh

echo '#include "global.h"'
echo '#include "main.h"'
echo '#include "malloc.h"'
echo '#include "random.h"'
echo '#include "sprite.h"'
echo '#include "test/test.h"'

currIt=-1

for file in graphics/**/*/anim_front.4bpp.lz; do
    ((currIt+=1))
    fileUncomp=${file::-3}
    echo 'static const u32 animFront'$currIt'[] = INCBIN_U32("'$fileUncomp'.lz");'
done

for file in graphics/**/*/front.4bpp.lz; do
    ((currIt+=1))
    fileUncomp=${file::-3}
    echo 'static const u32 animFront'$currIt'[] = INCBIN_U32("'$fileUncomp'.lz");'
done

for file in graphics/**/*/back.4bpp.lz; do
    ((currIt+=1))
    fileUncomp=${file::-3}
    echo 'static const u32 animFront'$currIt'[] = INCBIN_U32("'$fileUncomp'.lz");'
done

for file in graphics/**/*/anim_frontf.4bpp.lz; do
    ((currIt+=1))
    fileUncomp=${file::-3}
    echo 'static const u32 animFront'$currIt'[] = INCBIN_U32("'$fileUncomp'.lz");'
done

for file in graphics/**/*/frontf.4bpp.lz; do
    ((currIt+=1))
    fileUncomp=${file::-3}
    echo 'static const u32 animFront'$currIt'[] = INCBIN_U32("'$fileUncomp'.lz");'
done

for file in graphics/**/*/backf.4bpp.lz; do
    ((currIt+=1))
    fileUncomp=${file::-3}
    echo 'static const u32 animFront'$currIt'[] = INCBIN_U32("'$fileUncomp'.lz");'
done

echo ''
((currIt+=1))
echo 'static const u32 *allFiles['$currIt'] ='
((currIt-=1))
echo '{'
for i in {0..$currIt}
do
    echo '    ['$i'] = animFront'$i','
done
echo '};'
echo ''

echo 'TEST("Compression test: all sprites, lz")'
echo '{'
echo '    u32 totalTime = 0;'
echo '    for (u32 j = 0; j < '$currIt'; j++)'
echo '    {'
echo '        const u32 *file = allFiles[j];'
echo '        u32 imageSize = GetDecompressedDataSize(file);'
echo '        u32 *compBuffer = Alloc(imageSize);'
echo '        CycleCountStart();'
echo '        DecompressDataWithHeaderWram(file, compBuffer);'
echo '        u32 timeTaken = CycleCountEnd();'
echo '        totalTime += timeTaken;'
echo '        Free(compBuffer);'
echo '    }'
echo '    DebugPrintf("Total time: %u", totalTime);'
echo '}'
