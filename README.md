# Synthlib2parser #

A parser for synthlib2 file format. This is the file format used in [sygus.org]

## Structure

The project has two main branches:
  - `master`. Lightweight and stable version. It's based on the parser that we 
  extracted from [esolver].
  - `next`. Collects more symbols data. It based on the official version [synthlib2parser].
  

## Dependencies

We depend on Boost `program_options`


  [esolver]: https://github.com/rishabhs/sygus-comp14/tree/master/solvers/enumerative
  [sygus.org]: http://sygus.org/
  [synthlib2parser]: https://github.com/rishabhs/sygus-comp14/tree/master/parser/synthlib2parser
