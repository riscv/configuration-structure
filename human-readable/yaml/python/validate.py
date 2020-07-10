import json
import jsonschema
import sys
import yaml
from yamllint import linter as yaml_linter
from yamllint.config import YamlLintConfig

# Get arguments.
schema_filename = sys.argv[1]
yaml_filename = sys.argv[2]

# Parse inputs.
with open(schema_filename, 'r') as schema_file, open(yaml_filename, 'r') as yaml_file:
    # Lint YAML source.
    yamllint_conf = YamlLintConfig("extends: default")
    errors = list(yaml_linter.run(input=yaml_file, conf=yamllint_conf))
    if errors:
        print(errors)
        print("yamllint failed for:", yaml_filename)
        sys.exit(1)
    yaml_file.seek(0)

    # Parse configuration.
    instance = yaml.safe_load(yaml_file)

    # Parse schema.
    schema = json.load(schema_file)

    # Validate YAML source against schema.
    jsonschema.validate(instance=instance, schema=schema)
