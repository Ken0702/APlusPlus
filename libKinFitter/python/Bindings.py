# @file SUSYTools/python/Bindings.py
# @author Renaud Bruneliere adapted from Sebastien Binet <binet@cern.ch> code
# @purpose load the python bindings to SUSYTools' functions
#          this is to workaround a limitation of PyCintex which cannot (yet?)
#          autoload free functions (only classes)

def _installBindings():
    """load the python bindings to SUSYTools' function, at module import
    """
    import ROOT
    import PyCintex
    PyCintex.Cintex.Enable ()
    ## load the dictionary:
    PyCintex.loadDictionary('libKinFitterDict')
    
    return

# execute at module import
_installBindings()

# clean-up
del _installBindings
