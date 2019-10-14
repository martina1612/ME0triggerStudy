if [ "$1" != "prompt" ] && [ "$1" != "disp" ] && [ "$2" != "digi" ] && [ "$2" != "pad" ] ; then
  echo -e "Usage:\n$(echo $0) <type> <tag> <version> [<folder>]"
  echo -e "<type>\t\tprompt,disp\n<tag>\t\tdigi,pad\n<version>\tanalyzer version number\n<folder>\tDestination folder to contain output files. <type>, <tag>, <version> and current timestamp will be added to the given name.\n\t\tIf it doesn't exist it will be created.\n\t\tIf not specified it will be created using current timestamp."
  exit
fi

if [ "$4" != "" ] ; then
  folder="$(echo $4)_"
fi

folder="$(echo $folder)globalPhi_$(echo $1)Mu_$(echo $2)Based_v$(echo $3)_$(date +%d%m%Y-%H%M%S)"
echo "Creating folder $folder"
mkdir -p $folder


#root -l analyzeME0_globalPhi.C\(\"$1\",\"$2\",\"$3\"\, \"$4\"\)
root -l -b -q analyzeME0_globalPhi.C\(\"$1\",\"$2\",\"$3\",\"$(echo $folder)\"\)


# MERGE PLOTS INTO FEW SLIDES
mkdir -p $(echo $folder)/summary

files="$(echo $folder)/comparisonAlpha_*.png"
  montage -mode concatenate $(echo $files) -tile 3x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "Alpha comparison (segment vs sim hit)" ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/comparisonAlpha.pdf

files="$(echo $folder)/comparisonDeltaPhi_*.png"
  montage -mode concatenate $(echo $files) -tile 3x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "DeltaPhi comparison (segment vs sim hit)" ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/comparisonDeltaPhi.pdf

files="$(echo $folder)/alphaROC_*.png"
  montage -mode concatenate $(echo $files) -tile 3x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "alpha ROC curves" ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/alphaROC.pdf

files="$(echo $folder)/deltaPhiROC_*.png"
title="DeltaPhi ROC curves"
  montage -mode concatenate $(echo $files) -tile 3x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "DeltaPhi ROC curves" ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/deltaPhiROC.pdf
files="$(echo $folder)/allAlpha_*.png"
  montage -mode concatenate $(echo $files) -tile 2x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "Alpha" ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/allAlpha.pdf
files="$(echo $folder)/allDeltaPhi_*.png"
  montage -mode concatenate $(echo $files) -tile 2x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "DeltaPhi" ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/allDeltaPhi.pdf
files="$(echo $folder)/allAlphaCum_*.png"
  montage -mode concatenate $(echo $files) -tile 2x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "Alpha Cumulatives" ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/allAlphaCum.pdf
files="$(echo $folder)/allDeltaPhiCum_*.png"
  montage -mode concatenate $(echo $files) -tile 2x2 -geometry 430x  out.pdf
  convert out.pdf  -splice 0x40 out2.pdf
  convert out2.pdf -pointsize 22 -fill blue -draw 'text 10,30 "DeltaPhi Cumulatives ' out3.pdf
  mv out3.pdf $(echo $folder)/summary/allDeltaPhiCum.pdf

rm out.pdf out2.pdf

