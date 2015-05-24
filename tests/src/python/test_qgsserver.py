# -*- coding: utf-8 -*-
"""QGIS Unit tests for QgsServer.

.. note:: This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
"""
__author__ = 'Alessandro Pasotti'
__date__ = '25/05/2015'
__copyright__ = 'Copyright 2015, The QGIS Project'
# This will get replaced with a git SHA1 when you do a git archive
__revision__ = '$Format:%H$'

import unittest
from qgis.server import QgsServer


class TestQgsServer(unittest.TestCase):

    def setUp(self):
        """Create the server instance"""
        self.server = QgsServer()

    def test_api(self):
        """Using an empty query string (returns an XML exception)
        we are going to test if headers and body are returned correctly"""
        # Test as a whole
        response = str(self.server.handleRequest())
        expected = 'Content-Type: text/xml; charset=utf-8\nContent-Length: 206\n\n<ServiceExceptionReport version="1.3.0" xmlns="http://www.opengis.net/ogc">\n <ServiceException code="Service configuration error">Service unknown or unsupported</ServiceException>\n</ServiceExceptionReport>\n'
        self.assertEqual(response, expected)
        # Test header
        response = str(self.server.handleRequestGetHeaders())
        expected = 'Content-Type: text/xml; charset=utf-8\nContent-Length: 206\n\n'
        self.assertEqual(response, expected)
        # Test body
        response = str(self.server.handleRequestGetBody())
        expected = '<ServiceExceptionReport version="1.3.0" xmlns="http://www.opengis.net/ogc">\n <ServiceException code="Service configuration error">Service unknown or unsupported</ServiceException>\n</ServiceExceptionReport>\n'
        self.assertEqual(response, expected)


if __name__ == '__main__':
    unittest.main()
