import os
from SCons.Script import *
from SCons.Builder import Builder
from SCons.Scanner.C import CScanner

class ToolInstrumentWarning(SCons.Warnings.Warning):
    pass

class MissingRequiredVariable(ToolInstrumentWarning):
    def __init__(self, var):
        super(ToolInstrumentWarning, self).__init__("Missing required variable '%s'" % var)

class AppNotFound(ToolInstrumentWarning):
    def __init__(self, app):
        super(ToolInstrumentWarning, self).__init__("Could not find application '%s'" % app)


def _detect_app(env, var_name, app_name):
    """ Try to detect a specific application """
    try:
        return env[var_name]
    except KeyError:
        pass

    app = env.WhereIs(app_name)
    if app:
        return app

    raise SCons.Errors.StopError(AppNotFound(app_name))
    return None


_instrument_llvm_builder = SCons.Builder.Builder(
    action = SCons.Action.Action('$CLANG -S -emit-llvm -g $CFLAGS $CPP_PATH -c $SOURCE -o $TARGET'),
    src_suffix = '$INSTR_SOURCE_SUFFIX',
    suffix = '$INSTR_LLVM_SUFFIX',
    single_source = 1)

_instrument_llvm_instrument_builder = SCons.Builder.Builder(
    action = SCons.Action.Action('$OPT -S -f --load $LLVM_INSTRUMENT_LIB $LLVM_INSTRUMENT_PASSES $SOURCE -o $TARGET'),
    src_suffix = '$INSTR_LLVM_SUFFIX',
    suffix = '$INSTR_LLVM_INSTR_SUFFIX',
    single_source = 1)

_instrument_asm_builder = SCons.Builder.Builder(
    action = SCons.Action.Action('$LLC -o $TARGET < $SOURCE'),
    src_suffix = '$INSTR_LLVM_INSTR_SUFFIX',
    suffix = '$INSTR_ASM_SUFFIX',
    single_source = 1)

_instrument_linker_builder = SCons.Builder.Builder(
    action = SCons.Action.Action('$GCC $LINK_PATH $LINKFLAGS -o $TARGET $SOURCES'),
    src_suffix = '$INSTR_ASM_SUFFIX')

def Instrument(env, target, source=None, passes="-instrument_prepare", *args, **kw):
    env['LLVM_INSTRUMENT_PASSES'] = passes

    source_suffix = env.subst('$INSTR_SOURCE_SUFFIX')

    llvm_suffix = env.subst('$INSTR_LLVM_SUFFIX')
    llvm_instr_suffix = env.subst('$INSTR_LLVM_INSTR_SUFFIX')
    asm_suffix = env.subst('$INSTR_ASM_SUFFIX')

    llvm_instrument_lib = env.subst('$LLVM_INSTRUMENT_LIB')
    instrument_lib = env.subst('$INSTRUMENT_LIB')

    scanner = CScanner()
    result = []
    asm_objects = []
    for s in source:
        stem = src = str(s)
        if src.endswith(source_suffix):
            stem = src[:-len(source_suffix)]
        else:
            stem = src

        llvm_obj = _instrument_llvm_builder.__call__(env,
                                                     stem + llvm_suffix,
                                                     src,
                                                     **kw)

        llvm_instrument_obj = _instrument_llvm_instrument_builder.__call__(env,
                                                                           stem + llvm_instr_suffix,
                                                                           stem + llvm_suffix,
                                                                           **kw)

        asm_obj = _instrument_asm_builder.__call__(env,
                                                   stem + asm_suffix,
                                                   stem + llvm_instr_suffix,
                                                   **kw)


        env.Depends(llvm_obj, scanner.scan(s))
        env.Depends(llvm_instrument_obj, llvm_obj)
        env.Depends(llvm_instrument_obj, llvm_instrument_lib)
        env.Depends(asm_obj, llvm_instrument_obj)

        asm_objects.extend(asm_obj)

    env.Requires(target, instrument_lib)
    target_obj = _instrument_linker_builder.__call__(env, target, asm_objects, **kw)

    return target_obj


_required_vars = ['LLVM_INSTRUMENT_LIB',
                  'INSTRUMENT_LIB']

_instrument_apps = [('CLANG', 'clang'),
                    ('OPT', 'opt'),
                    ('LLC', 'llc'),
                    ('GCC', 'g++')]
def generate(env):
    """Add Builders and construction variables to the Environment."""
    exists(env)

    for var_name, app_name in _instrument_apps:
        env[var_name] = _detect_app(env, var_name, app_name)

    env.SetDefault(
        # Suffixes/prefixes
        INSTR_SOURCE_SUFFIX = '.cpp',
        INSTR_LLVM_SUFFIX = '.bc',
        INSTR_LLVM_INSTR_SUFFIX = '.bci',
        INSTR_ASM_SUFFIX = '.s')
    env.AddMethod(Instrument, "Instrument");



def exists(env):
    for var_name in _required_vars:
        if var_name not in env:
            raise SCons.Errors.StopError(MissingRequiredVariable(var_name))
    for var_name, app_name in _instrument_apps:
        _detect_app(env, var_name, app_name)
