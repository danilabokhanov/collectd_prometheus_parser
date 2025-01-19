## Prometheus Parser
This is a parser part of [prometheus collectd read plugin](https://github.com/collectd/collectd/pull/4338). The parser is written with respect to [official prometheus exposition
format](https://github.com/prometheus/docs/blob/main/content/docs/instrumenting/exposition_formats.md)

### Deployment
```sudo apt-get update```

```sudo apt-get install -y bison flex gcc libfl-dev```

```./build.sh```

### Tests
```python -m pip install --upgrade pip```

```pip install pytest```

```pytest test_parser.py```
