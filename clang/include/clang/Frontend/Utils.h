//===--- Utils.h - Misc utilities for the front-end -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This header contains miscellaneous utilities for various front-end actions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_FRONTEND_UTILS_H
#define LLVM_CLANG_FRONTEND_UTILS_H

#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
class Triple;
}

namespace clang {
class ASTConsumer;
class Decl;
class DependencyOutputOptions;
class Diagnostic;
class DiagnosticOptions;
class HeaderSearch;
class HeaderSearchOptions;
class IdentifierTable;
class LangOptions;
class MinimalAction;
class Preprocessor;
class PreprocessorOptions;
class PreprocessorOutputOptions;
class SourceManager;
class Stmt;
class TargetInfo;

class MacroBuilder {
  llvm::raw_ostream &Out;
public:
  MacroBuilder(llvm::raw_ostream &Output) : Out(Output) {}

  /// Append a #define line for macro of the form "#define Name Value\n".
  void defineMacro(const llvm::Twine &Name, const llvm::Twine &Value = "1") {
    Out << "#define " << Name << ' ' << Value << '\n';
  }

  /// Append a #undef line for Name.  Name should be of the form XXX
  /// and we emit "#undef XXX".
  void undefineMacro(const llvm::Twine &Name) {
    Out << "#undef " << Name << '\n';
  }

  /// Directly append Str and a newline to the underlying buffer.
  void append(const llvm::Twine &Str) {
    Out << Str << '\n';
  }
};

/// Normalize \arg File for use in a user defined #include directive (in the
/// predefines buffer).
std::string NormalizeDashIncludePath(llvm::StringRef File);

/// Apply the header search options to get given HeaderSearch object.
void ApplyHeaderSearchOptions(HeaderSearch &HS,
                              const HeaderSearchOptions &HSOpts,
                              const LangOptions &Lang,
                              const llvm::Triple &triple);

/// InitializePreprocessor - Initialize the preprocessor getting it and the
/// environment ready to process a single file.
void InitializePreprocessor(Preprocessor &PP,
                            const PreprocessorOptions &PPOpts,
                            const HeaderSearchOptions &HSOpts);

/// ProcessWarningOptions - Initialize the diagnostic client and process the
/// warning options specified on the command line.
bool ProcessWarningOptions(Diagnostic &Diags, const DiagnosticOptions &Opts);

/// DoPrintPreprocessedInput - Implement -E mode.
void DoPrintPreprocessedInput(Preprocessor &PP, llvm::raw_ostream* OS,
                              const PreprocessorOutputOptions &Opts);

/// RewriteMacrosInInput - Implement -rewrite-macros mode.
void RewriteMacrosInInput(Preprocessor &PP, llvm::raw_ostream* OS);

/// RewriteMacrosInInput - A simple test for the TokenRewriter class.
void DoRewriteTest(Preprocessor &PP, llvm::raw_ostream* OS);

/// CreatePrintParserActionsAction - Return the actions implementation that
/// implements the -parse-print-callbacks option.
MinimalAction *CreatePrintParserActionsAction(Preprocessor &PP,
                                              llvm::raw_ostream* OS);

/// CheckDiagnostics - Gather the expected diagnostics and check them.
bool CheckDiagnostics(Preprocessor &PP);

/// AttachDependencyFileGen - Create a dependency file generator, and attach
/// it to the given preprocessor.  This takes ownership of the output stream.
void AttachDependencyFileGen(Preprocessor &PP,
                             const DependencyOutputOptions &Opts);

/// CacheTokens - Cache tokens for use with PCH. Note that this requires
/// a seekable stream.
void CacheTokens(Preprocessor &PP, llvm::raw_fd_ostream* OS);

}  // end namespace clang

#endif
