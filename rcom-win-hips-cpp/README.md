# Windows 11 - ROCm 7.1 - HIPs - c++ VSCode Setup

- Download Visual Studio Community 2022
    - Install with Desktop C++ Development Packages
- Download and install VS Code
- Validate you have Coderunner extension installed
- Validate you have c++ extensions installed
- Download and Install ROCm Windows HIPs SDK 7.1
- Reboot

Edit environment vars in windows, you should see HIP_PATH already installed.

Add to the System Variables PATH:

- %HIP_PATH%\bin
- %HIP_PATH%\lib

In VS Code we will need to edit the coderunner setup:

- Edit: Core-runner Executor Map JSON

There are a couple pertinent lines:

- `"cpp": "cd $dir && clang++ -O3 --offload-arch=native $fileName -o $fileNameWithoutExt.exe && .\\$fileNameWithoutExt.exe",`
- `"hip": "cd $dir && hipcc -O3 --offload-arch=native $fileName -o $fileNameWithoutExt.exe && .\\$fileNameWithoutExt.exe",`

and

```json
  "files.associations": {
    "*.hip": "cpp"
  },
```

Full file:

```json
{
  "security.workspace.trust.untrustedFiles": "open",
  "powershell.integratedConsole.showOnStartup": false,
  "editor.copyWithSyntaxHighlighting": false,
  "github.copilot.enable": {
    "*": "true",
    "plaintext": true,
    "markdown": true,
    "scminput": false,
    "lisp": "false"
  },
  "github.copilot.editor.enableAutoCompletions": false,
  "workbench.iconTheme": "vscode-icons",
  "files.associations": {
    "*.hip": "cpp"
  },
  "editor.acceptSuggestionOnEnter": "off",
  "chat.editing.alwaysSaveWithGeneratedChanges": true,
  "editor.fontFamily": "Brass Mono Regular",
  "terminal.integrated.defaultProfile.windows": "Windows PowerShell",
  "code-runner.executorMap": {
    

    "javascript": "clear && node",
    "java": "cd $dir && javac $fileName && java $fileNameWithoutExt",
    "c": "clear && cd $dir && gcc $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    "zig": "clear && zig run",
    "cpp": "cd $dir && clang++ -O3 --offload-arch=native $fileName -o $fileNameWithoutExt.exe && .\\$fileNameWithoutExt.exe",
    "hip": "cd $dir && hipcc -O3 --offload-arch=native $fileName -o $fileNameWithoutExt.exe && .\\$fileNameWithoutExt.exe",
    "objective-c": "cd $dir && gcc -framework Cocoa $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    "php": "php",
    "python": "clear && python -u",
    "perl": "perl",
    "perl6": "perl6",
    "ruby": "ruby",
    "go": "clear && go run",
    "lua": "clear && lua",
    "groovy": "groovy",
    "powershell": "clear && powershell -ExecutionPolicy ByPass -File",
    "bat": "cmd /c",
    "shellscript": "bash",
    "fsharp": "fsi",
    "csharp": "scriptcs",
    "vbscript": "cscript //Nologo",
    "typescript": "ts-node",
    "coffeescript": "coffee",
    "scala": "scala",
    "swift": "swift",
    "julia": "julia",
    "crystal": "crystal",
    "ocaml": "ocaml",
    "r": "Rscript",
    "applescript": "osascript",
    "clojure": "lein exec",
    "haxe": "haxe --cwd $dirWithoutTrailingSlash --run $fileNameWithoutExt",
    "rust": "cd $dir && rustc $fileName && $dir$fileNameWithoutExt",
    "racket": "racket",
    "scheme": "csi -script",
    "ahk": "autohotkey",
    "autoit": "autoit3",
    "dart": "dart",
    "pascal": "cd $dir && fpc $fileName && $dir$fileNameWithoutExt",
    "d": "cd $dir && dmd $fileName && $dir$fileNameWithoutExt",
    "haskell": "runghc",
    "nim": "nim compile --verbosity:0 --hints:off --run",
    "lisp": "clear && sbcl --script",
    "kit": "kitc --run",
    "v": "v run",
    "sass": "sass --style expanded",
    "scss": "scss --style expanded",
    "less": "cd $dir && lessc $fileName $fileNameWithoutExt.css",
    "FortranFreeForm": "cd $dir && gfortran $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    "fortran-modern": "cd $dir && gfortran $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    "fortran_fixed-form": "cd $dir && gfortran $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    "fortran": "cd $dir && gfortran $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    "sml": "cd $dir && sml $fileName",
    "mojo": "mojo run",
    "erlang": "escript",
    "spwn": "spwn build",
    "pkl": "cd $dir && pkl eval -f yaml $fileName -o $fileNameWithoutExt.yaml",
    "gleam": "gleam run -m $fileNameWithoutExt"
  },
  "code-runner.runInTerminal": true,
  "editor.unicodeHighlight.ambiguousCharacters": false,
  "jupyter.runStartupCommands": "",
  "terminal.integrated.defaultProfile.linux": "",
  "terminal.integrated.env.windows": {},
  "vsicons.dontShowNewVersionMessage": true,
  "editor.fontSize": 16,
  "editor.wordWrap": "on",
  "redhat.telemetry.enabled": false,
  "extensions.ignoreRecommendations": true,
  "workbench.colorTheme": "Dark Modern (OLED Black) [Fuchsia]",
  "github.copilot.nextEditSuggestions.enabled": true,
  "explorer.confirmDragAndDrop": false,
  "remote.SSH.remotePlatform": {
    "10.0.0.102": "linux"
  },
  "editor.inlineSuggest.edits.allowCodeShifting": "never",
  "yaml.schemas": {
    "file:///home/pcarroll/.vscode/extensions/continue.continue-1.2.22-linux-x64/config-yaml-schema.json": [
      ".continue/**/*.yaml"
    ],
    "file:///c%3A/Users/phill/.vscode/extensions/continue.continue-1.2.22-win32-x64/config-yaml-schema.json": [
      ".continue/**/*.yaml"
    ],
    "file:///c%3A/Users/continue.continue-1.3.38-win32-x64/config-yaml-schema.json": [
      ".continue/**/*.yaml"
    ],
    "file:///c%3A/Users/phil/.vscode/extensions/continue.continue-1.2.22-win32-x64/config-yaml-schema.json": [
      ".continue/**/*.yaml"
    ],
    "file:///c%3A/Users/user/.vscode/extensions/continue.continue-1.2.22-win32-x64/config-yaml-schema.json": [
      ".continue/**/*.yaml"
    ],
    "file:///c%3A/Users/phil/.vscode/extensions/continue.continue-1.2.24-win32-x64/config-yaml-schema.json": [
      ".continue/**/*.yaml"
    ]
  },
  "terminal.integrated.stickyScroll.enabled": false,
  "terminal.integrated.suggest.quickSuggestions": {
    "commands": "on",
    "arguments": "on",
    "unknown": "on"
  },
  "emmet.useInlineCompletions": true,
  "editor.inlineSuggest.minShowDelay": 500,
  "chat.disableAIFeatures": true,
  "code-runner.executorMapByGlob": {
    "pom.xml": "cd $dir && mvn clean package"
  },
  "files.autoSave": "afterDelay"
}
```