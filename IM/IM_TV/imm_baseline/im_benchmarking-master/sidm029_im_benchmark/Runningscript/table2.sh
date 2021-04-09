
celfic=$(python processtable2.py celf ic)
celfwc=$(python processtable2.py celf wc)
celflt=$(python processtable2.py celf lt)


celfppic=$(python processtable2.py celfpp ic)
celfppwc=$(python processtable2.py celfpp wc)
celfpplt=$(python processtable2.py celfpp lt)


easyimic=$(python processtable2.py easyim ic)
easyimwc=$(python processtable2.py easyim wc)
easyimlt=$(python processtable2.py easyim lt)

immic=$(python processtable2.py imm ic)
immwc=$(python processtable2.py imm wc)
immlt=$(python processtable2.py imm lt)

timic=$(python processtable2.py tim ic)
timwc=$(python processtable2.py tim wc)
timlt=$(python processtable2.py tim lt)


pmcic=$(python processtable2.py pmc ic)
pmcwc=$(python processtable2.py pmc wc)


sgic=$(python processtable2.py sg ic)
sgwc=$(python processtable2.py sg wc)

echo "TECH IC WC LT"
echo "CELF "$celfic" "$celfwc" "$celflt
echo "CELF++ "$celfppic" "$celfppwc" "$celfpplt
echo "EaSyIM "$easyimic" "$easyimwc" "$easyimlt
echo "Imrank(l=1) 10 10 NA"
echo "Imrank(l=2) 10 10 NA"
echo "PMC "$pmcic" "$pmcwc" NA"
echo "StaticGreedy "$sgic" "$sgwc" NA"
echo "IMM "$immic" "$immwc" "$immlt
echo "TIM "$timic" "$timwc" "$timlt
