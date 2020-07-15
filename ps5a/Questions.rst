

- At what problem size do the answers between the computed norms start to differ?
    The two_norm forward and reverse implementations begin to differ around N = 10050.
    The two_norm forward and sorted implementations begin to differ around N = 10060.
    However, going up to N=100000, only the sorted norm implementation produces a difference. It appears, not
    simply input magnitude effects whether or not there is a difference in a given implementation. For instance at
    N = 100500, both reversed and sorted implementations show a difference in results.
    I find it interesting that the different implementations don't diverge at the same N. It seems picking simple powers
    of ten won't always produce a difference between norm implementations.


- How do the absolute and relative errors change as a function of problem size?
    At N=10060, the absolute difference for both reversed and sorted is on the order of 1.4e-14 and
    the relative differences for both are on the order of 1.2e-16.

    At N=2e5, the relative difference for reversed is 6.6e-16 and the absolute difference is 3.4e-13.
    For sorted, the relative difference for reversed is 2.2e-16 and the absolute difference is 1.1e-13.

    At N=2e6, the relative difference for reversed is 8.4e-16 and the absolute difference is 1.4e-12.
    For sorted, the relative difference for reversed is 1.5e-15 and the absolute difference is 2.5e-12.

    At N=2e7, the relative difference for reversed is 2.0e-14 and the absolute difference is 3.2e-11.
    For sorted, the relative difference for reversed is 2.1e-15 and the absolute difference is 1.1e-11.

    At N=2e8, the relative difference for reversed is 2.0e-14 and the absolute difference is 3.3e-10.
    For sorted, the relative difference for reversed is 2.2e-14 and the absolute difference is 3.5e-10.

    It appears that as the problem size increases by an order of magnitude, the relative and absolute errors
    also increase by an order of magnitude. So the relative error seems to be directly proportional to problem size. 

- Does the Vector class behave strictly like a member of an abstract vector class?
    For an abstract mathematical vector, the sorted and reversed 2-norm implementations should behave the
    same as the standard 2-norm definition.  The order of the summed elements doesn't matter as long as all elements
    are included once. Our Vector class does not behave strictly as an abstract vector would, especially for large 
    numbers of elements.  There should not be a difference between these norm implementations.

- Do you have any concerns about this kind of behavior?
    This behavior becomes a problem when multiple such vector or matrix operations are performed.  Additional numerical error is introduced
    every time an operation of this type is performed.  For large scale applications doing millions of matrix/vector calculations this
    numerical error quickly accumulates and distorts results if not accounted for. 


