## Getting Started
```sh
# Install msa-tk
git clone https://github.com/kits-hub/msa-tk
cd msa-tk && make

# genrate identity matrix
./msa-tk ident_matrix test-data/msa.fa

# find nearest k neighbor via identity ( identity = match/length )
./msa-tk  neighbor -n 1 -i 0.3   test-data/msa.fa test-data/list
```
