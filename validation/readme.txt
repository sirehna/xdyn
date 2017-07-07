To generate the reports, run

python3 JinJaReport.py  -p ~/simulator/validation/gir.md -r toto.docx -k

from the spt/spt/report/extJinja2 directory.

Using Docker:

docker run --rm -it -w /work -v $(pwd):/work xdyn python3 /usr/local/lib/python3.4/dist-packages/spt/report/JinJaReport.py -p gir.md -r toto.docx -k

Using script

./report.sh -p gir.md -r toto.docx -k
