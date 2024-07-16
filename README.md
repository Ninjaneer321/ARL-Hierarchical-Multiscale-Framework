## ARL Hierarchical MultiScale Framework (ARL-HMS)

The ARL Hierarchical MultiScale Framework (ARL-HMS) is a software
library for development of multiscale models on heterogeneous
high-performance computing systems. Multiscale models are created in
ARL-HMS from an assembly of individual at-scale model components. A
runtime system adaptively evaluates at-scale model components in
parallel and performs data extraction necessary for
scale-bridging. Adaptive machine learning methods are also available
in ARL-HMS to significantly reduce computational expense.

The ARL-HMS software is implemented in C++ and includes bindings to
Python. It can incorporate individual at-scale model components
written in any programming language and can also incorporate
closed-source or proprietary software allowing users to leverage
existing complex at-scale models for multiscale model development.

A User's Guide to ARL-HMS is included in this software repository
(ARL-TR-9820.pdf). The User's Guide documents the steps necessary to
develop multiscale models with ARL-HMS. It includes an overview of the
mathematical approach to multiscale modeling the framework adopts, the
steps necessary to incorporate at-scale models, use of the ARL-HMS
Broker to perform at-scale model evaluation and data extraction, and
use of the adaptive surrogate modeling capabilities implemented in
ARL-HMS. A simple example ARL-HMS application, implemented in both C++
and Python, is included that performs Monte Carlo estimation of
$\pi$. The example application is also included in the repository in
the examples/mcpi directory.

### ARL-HMS Dependencies and Compilation Steps

ARL-HMS requires a compiled Boost C++ installation for data
serialization and shared pointer capabilities.

Optional dependencies include a Message Passing Interface (MPI)
library to enable MPI communication and fan-in data communication
between ARL-HMS components. A Python installation is required to build
the ARL-HMS Python bindings. The Adaptive Sampling Framework (ASF)
library from LLNL (https://github.com/exmatex/aspa) is also required
to use the Kriging database interpolation method.

ARL-HMS uses a standard autotools installation procedure (configure,
make, make install) documented in Section 5 of the ARL-HMS User's
Guide.

### ARL-HMS Citations

We request that if you publish results that make use of the ARL-HMS
software, please cite the following paper and ARL-HMS User's Guide:

@article{Knap2016,
  author = {Knap, J. and Spear, C. and Leiter, K. and Becker, R. and Powell, D.},
  title = {A computational framework for scale-bridging in multi-scale simulations},
  journal = {International Journal for Numerical Methods in Engineering},
  volume = {108},
  number = {13},
  pages = {1649-1666},
  doi = {https://doi.org/10.1002/nme.5270},
  year = {2016}
}

@techreport{Leiter2023,
  institution = {{DEVCOM} Army Research Laboratory},
  title={User's Guide for the {Hierarchical MultiScale Framework} ({HMS})},
  author={Leiter, Kenneth W and Crone, Joshua C and Knap, Jaroslaw},
  year={2023},
  month={Oct},
  number={ARL-TR-9820}
}

### License

ARL Hierarchical MultiScale Framework (ARL-HMS) is licensed under the
Creative Commons Zero 1.0 Universal (CC0 1.0) license. Please see
[LICENSE](LICENSE) for details.
