try:
    from setup import setup
except ImportError:
    from distutils.core import setup
    
config = {
    'description': 'more auto, more fast.',
    'author': 'Zhang Xiuhai',
    'url': 'URL to get it at.',
    'download_url': 'Where to download it.',
    'author_email': 'xiuhai5052@hotmail.com.',
    'version': '0.1',
    'install_requires': ['nose'],
    'packages': ['NAME'],
    'scripts': [],
    'name': 'Automation Cloud'
}

setup(**config)