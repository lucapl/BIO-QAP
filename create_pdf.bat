set "REPORT_FILE=report"
py -m jupyter nbconvert --to markdown --no-input %REPORT_FILE%.ipynb && pandoc --toc %REPORT_FILE%.md -o %REPORT_FILE%.pdf --filter pandoc-crossref  --pdf-engine=xelatex