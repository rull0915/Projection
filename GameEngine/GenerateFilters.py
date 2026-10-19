#!/usr/bin/env python3
"""Generate a Visual Studio .vcxproj.filters file from a project's disk folders.

Usage:
  python generate_filters.py path/to/MyEngine.vcxproj
  python generate_filters.py path/to/MyEngine.vcxproj --root path/to/project/root
"""
from __future__ import annotations

import argparse
import uuid
import xml.etree.ElementTree as ET
from pathlib import Path

PROJECT_NS = "http://schemas.microsoft.com/developer/msbuild/2003"
ET.register_namespace("", PROJECT_NS)

COMPILE_EXTENSIONS = {".c", ".cc", ".cpp", ".cxx"}
HEADER_EXTENSIONS = {".h", ".hh", ".hpp", ".hxx", ".inl", ".ipp"}
RESOURCE_EXTENSIONS = {".rc", ".resx"}
SHADER_EXTENSIONS = {".hlsl", ".hlsli", ".fx", ".fxh"}
OTHER_EXTENSIONS = {".txt", ".json", ".xml", ".ini", ".csv", ".md"}


def tag(name: str) -> str:
    return f"{{{PROJECT_NS}}}{name}"


def guid_for_filter(name: str) -> str:
    # Stable GUID: unchanged filter names keep the same GUID between runs.
    return "{" + str(uuid.uuid5(uuid.NAMESPACE_URL, "vcxproj-filter:" + name)).upper() + "}"

def classify(path: Path) -> str:
    ext = path.suffix.lower()
    if ext in COMPILE_EXTENSIONS:
        return "ClCompile"
    if ext in HEADER_EXTENSIONS:
        return "ClInclude"
    if ext in RESOURCE_EXTENSIONS:
        return "ResourceCompile"
    if ext in SHADER_EXTENSIONS or ext in OTHER_EXTENSIONS:
        return "None"
    return "None"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("project", type=Path, help="Path to .vcxproj")
    parser.add_argument("--root", type=Path, help="Folder to scan (default: .vcxproj's parent)")
    parser.add_argument("--exclude", action="append", default=[],
                        help="Directory name to skip; can be specified multiple times")
    parser.add_argument("--dry-run", action="store_true", help="Print output path without writing")
    args = parser.parse_args()

    project = args.project.resolve()
    if not project.is_file() or project.suffix.lower() != ".vcxproj":
        parser.error(f"Not a .vcxproj file: {project}")

    root = (args.root.resolve() if args.root else project.parent)
    if not root.is_dir():
        parser.error(f"Scan root is not a directory: {root}")

    filters_path = project.with_suffix(".vcxproj.filters")
    excluded = {".git", ".vs", "Debug", "Release", "x64", "Win32", "__pycache__"}
    excluded.update(args.exclude)

    files: list[tuple[Path, str]] = []
    filters: set[str] = set()

    for file_path in sorted(root.rglob("*")):
        if not file_path.is_file():
            continue
        try:
            relative = file_path.relative_to(root)
        except ValueError:
            continue

        # Skip files inside excluded directories and Visual Studio project artifacts.
        if any(part in excluded for part in relative.parts[:-1]):
            continue
        if file_path.name in {project.name, filters_path.name}:
            continue
        if file_path.suffix.lower() in {".vcxproj", ".filters", ".sln", ".user", ".suo"}:
            continue

        item_type = classify(file_path)
        # Keep only source/header/resource/shader and common project-side text assets.
        if item_type == "None" and file_path.suffix.lower() not in (
            SHADER_EXTENSIONS | OTHER_EXTENSIONS
        ):
            continue

        rel = relative.as_posix()
        filter_name = str(relative.parent).replace("/", "\\")
        if filter_name == ".":
            filter_name = ""
        else:
            # Add every parent filter so nested filters appear correctly.
            parts = relative.parent.parts
            for i in range(1, len(parts) + 1):
                filters.add(str(Path(*parts[:i])).replace("/", "\\"))
        files.append((file_path, filter_name))

    project_element = ET.Element(tag("Project"))

    filter_group = ET.SubElement(project_element, tag("ItemGroup"))
    for filter_name in sorted(filters, key=lambda s: (s.count("/"), s.lower())):
        filter_element = ET.SubElement(filter_group, tag("Filter"), Include=filter_name)
        unique_id = ET.SubElement(filter_element, tag("UniqueIdentifier"))
        unique_id.text = guid_for_filter(filter_name)

    # Group items by MSBuild item type.
    by_type: dict[str, list[tuple[Path, str]]] = {}
    for file_path, filter_name in files:
        item_type = classify(file_path)
        by_type.setdefault(item_type, []).append((file_path, filter_name))

    for item_type in ("ClCompile", "ClInclude", "ResourceCompile", "None"):
        entries = by_type.get(item_type, [])
        if not entries:
            continue
        item_group = ET.SubElement(project_element, tag("ItemGroup"))
        for file_path, filter_name in entries:
            relative_to_project = __import__("os").path.relpath(file_path, project.parent).replace("/", "\\")
            item = ET.SubElement(item_group, tag(item_type), Include=relative_to_project)
            if filter_name:
                filter_element = ET.SubElement(item, tag("Filter"))
                filter_element.text = filter_name

    ET.indent(project_element, space="  ")
    xml_bytes = ET.tostring(project_element, encoding="utf-8", xml_declaration=True)
    output = xml_bytes.decode("utf-8") + "\n"

    if args.dry_run:
        print(f"Would write: {filters_path}")
        print(f"Found {len(files)} files and {len(filters)} filters.")
        return 0

    filters_path.write_text(output, encoding="utf-8")
    print(f"Generated: {filters_path}")
    print(f"Files: {len(files)}, filters: {len(filters)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
